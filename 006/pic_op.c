/*
 * Copyright (C) 2019 francis_hao <francis_hao@126.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

struct buffer_data 
{
    const uint8_t *ptr;
    size_t size; ///< size left in the buffer
};

static int read_packet(void *opaque, uint8_t *buf, int buf_size)
{
    struct buffer_data *bd = (struct buffer_data *)opaque;
	buf_size = FFMIN(buf_size, bd->size);
    if (!buf_size) {
        return AVERROR_EOF;
	}
	printf("ptr:%p size:%zu\n", bd->ptr, bd->size);
    /* copy internal buffer data to buf */
    memcpy(buf, bd->ptr, buf_size);
    bd->ptr  += buf_size;
    bd->size -= buf_size;
    return buf_size;
}

int pic_demux(AVFormatContext **fmt_ctx, const char *url)
{
	int ret = 0;
	ret = avformat_open_input(fmt_ctx, url, NULL, NULL);
	if (ret < 0) {
		printf("avformat_open_input error: %s\n", av_err2str(ret));
		return ret;
	}
	ret = avformat_find_stream_info(*fmt_ctx, NULL);
	if (ret < 0) {
		printf("avformat_find_stream_info error: %s\n", av_err2str(ret));
		avformat_close_input(fmt_ctx);
		return ret;
	}

	if ((*fmt_ctx)->nb_streams <= 0) {
		printf("there is no available stream:%d\n", (*fmt_ctx)->nb_streams);
		avformat_close_input(fmt_ctx);
		return -1;
	}
	return 0;
}

int pix_convert(AVFrame *frame, enum AVPixelFormat format, uint8_t *buffer, int size)
{
	int ret = 0;
	/*********************************sws_scale**********************************/
	struct SwsContext *sws_ctx = NULL;
	uint8_t *sws_dst_data[4];
	int sws_dst_linesize[4];
	enum AVPixelFormat dst_format = format;
	ret = av_image_alloc(sws_dst_data, sws_dst_linesize,
			frame->width, frame->height, dst_format, 1);
	if (ret < 0) {
		printf("av_image_alloc error: %s\n", av_err2str(ret));
		return ret;
	}
	//av_freep(&pointers[0]);
	sws_ctx = sws_getContext(frame->width, frame->height, (enum AVPixelFormat)frame->format,\
			frame->width, frame->height, dst_format,\
			SWS_BILINEAR, NULL, NULL, NULL);
	if (NULL == sws_ctx) {
		printf("av_image_alloc error: %s\n", av_err2str(ret));
		av_freep(&sws_dst_data[0]);
		return -1;
	}
	ret = sws_scale(sws_ctx, (const uint8_t * const*)frame->data, frame->linesize, 0, frame->height, sws_dst_data, sws_dst_linesize);
	/**********sws_ctx is not used below, so we can clean it ***************/
	sws_freeContext(sws_ctx);

	ret = av_image_copy_to_buffer(buffer, size, (const uint8_t * const*)sws_dst_data, sws_dst_linesize, dst_format, frame->width,\
			frame->height, 1);
	if (ret < 0) {
		printf("av_image_alloc error: %s\n", av_err2str(ret));
		av_freep(&sws_dst_data[0]);
		return ret;
	}
	return 0;
}

int write_raw_data(const char *file_name, const uint8_t * buffer, size_t size)
{
	/******************write to file************************/
	FILE *output_file = NULL;
	output_file = fopen(file_name, "w+");
	if (NULL == output_file) {
		perror("fopen");
		return -1;
	}
	if (fwrite(buffer, 1, size, output_file) < size) {
		perror("Failed to dump raw data");
		fclose(output_file);
		return -1;
	}
	fclose(output_file);
	return 0;
}

int pic_open_path(const char *path)
{
	int ret = 0;
	/******************avformat**************************/
	AVFormatContext *ifmt_ctx = NULL;
	ret = pic_demux(&ifmt_ctx, path);
	if (ret < 0) {
		printf("pic_demux error: %s\n", av_err2str(ret));
		return ret;
	}
	AVStream *stream = ifmt_ctx->streams[0];
	if ( AV_PIX_FMT_NONE == stream->codecpar->format) {
		printf("cannot recognized format: %d\n", stream->codecpar->format);
		ret = -1;
		goto ending1;
	}
	printf("w:%d, h:%d, pix_fmt:%d, %s\n", stream->codecpar->width, \
			stream->codecpar->height, stream->codecpar->format,\
			av_get_pix_fmt_name((enum AVPixelFormat)stream->codecpar->format));
	printf("demux is %s, codec name is %s\n", ifmt_ctx->iformat->name, avcodec_get_name(stream->codecpar->codec_id));
	/*************************read_frame*****************************/
	AVFrame *frame = NULL;
	AVPacket packet = { .data = NULL, .size = 0 };
	ret = av_read_frame(ifmt_ctx, &packet);
	if (ret < 0) {
		printf("avformat_open_input error: %s\n", av_err2str(ret));
		goto ending2;
	}
	//av_packet_unref(&packet)
	frame = av_frame_alloc();
	if (NULL == frame) {
		printf("av_frame_alloc error\n");
		ret = -1;
		goto ending2;
	}
	//  av_frame_free(frame);
	/**************************decode**********************************/
	AVCodec *dec = NULL;
	AVCodecContext *codec_ctx = NULL;
	dec = avcodec_find_decoder(stream->codecpar->codec_id);
	if (NULL == dec) {
		printf("can not find decoder for stream %d\n", 0);
		ret = -1;
		goto ending3;
	}
	codec_ctx = avcodec_alloc_context3(dec);
	if (NULL == codec_ctx) {
		printf("avcodec_alloc_context3 error\n");
		ret = -1;
		goto ending3;
	}
	// avcodec_free_context(&codec_ctx)
	ret = avcodec_open2(codec_ctx, NULL, NULL);
	if (ret < 0) {
		printf("avformat_open_input error: %s\n", av_err2str(ret));
		goto ending3;
	}
	ret = avcodec_send_packet(codec_ctx, &packet);
	if (ret < 0) {
		printf("avcodec_send_packet error: %s\n", av_err2str(ret));
		goto ending3;
	}

	ret = avcodec_receive_frame(codec_ctx, frame);
	if (ret < 0) {
		printf("avcodec_send_packet error: %s\n", av_err2str(ret));
		goto ending3;
	}
	int size = 0;
	size = av_image_get_buffer_size((enum AVPixelFormat)frame->format, frame->width, frame->height, 1);
	printf("size is %d\n", size);

	/*********************************sws_scale**********************************/
	enum AVPixelFormat dst_format = AV_PIX_FMT_NV12;
	size = av_image_get_buffer_size(dst_format, frame->width, frame->height, 1);
	printf("pix convert dst size is %d\n", size);
	uint8_t *buffer = (uint8_t *)malloc(size);
	if (NULL == buffer) {
		perror("malloc");
		ret = -1;
		goto ending3;
	}
	ret = pix_convert(frame, dst_format, buffer, size);
	if (ret < 0) {
		goto ending4;
	}
	/******************write to file************************/
	ret = write_raw_data("aa.yuv", buffer, size);
	/*******************end**************************/
ending4:
	free(buffer);
ending3:
	if (NULL != codec_ctx) {
		avcodec_free_context(&codec_ctx);
	}
ending2:
	if (NULL != frame) {
		av_frame_free(&frame);
	}
ending1:
	if (NULL != ifmt_ctx) {
		avformat_close_input(&ifmt_ctx);
	}
	return ret;
}

