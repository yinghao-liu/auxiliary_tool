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
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixdesc.h>
#include <libavutil/opt.h>

int pic_open(const char *path)
{
	int ret = 0;
	AVFormatContext *ifmt_ctx = NULL;
	//AV_PIX_FMT_NV12;
	ret = avformat_open_input(&ifmt_ctx, path, NULL, NULL);
	if (ret < 0){
		printf("avformat_open_input error: %s\n", av_err2str(ret));
		return -1;
	}
	ret = avformat_find_stream_info(ifmt_ctx, NULL);
	if (ret < 0){
		printf("avformat_open_input error: %s\n", av_err2str(ret));
		return -1;
	}
	printf("w:%d, h:%d, pix_fmt:%d\n", ifmt_ctx->streams[0]->codecpar->width, ifmt_ctx->streams[0]->codecpar->height, ifmt_ctx->streams[0]->codecpar->format);
	if (ret < 0){
		printf("avformat_open_input error: %s\n", av_err2str(ret));
		return -1;
	}

	AVStream *stream = ifmt_ctx->streams[0];
	if ( AV_PIX_FMT_NONE == stream->codecpar->format)
	{
		printf("error pix_format: %d\n", stream->codecpar->format);
		return -1;
	}
	/*************************read_frame*****************************/
	AVFrame *frame = NULL;
	AVPacket packet = { .data = NULL, .size = 0 };
	ret = av_read_frame(ifmt_ctx, &packet);
	if (ret < 0){
		printf("avformat_open_input error: %s\n", av_err2str(ret));
		return -1;
	}
	frame = av_frame_alloc();
	if (NULL == frame){
		printf("av_frame_alloc error\n");
		return -1;
	}
	/**************************decode**********************************/
	AVCodec *dec = NULL;
	AVCodecContext *codec_ctx = NULL;
	dec = avcodec_find_decoder(stream->codecpar->codec_id);
	if (NULL == dec){
		printf("can not find decoder for stream %d\n", 0);
		return -1;
	}
	codec_ctx = avcodec_alloc_context3(dec);
	if (NULL == codec_ctx){
		printf("avcodec_alloc_context3 error\n");
		return -1;
	}
	ret = avcodec_open2(codec_ctx, NULL, NULL);
	if (ret < 0){
		printf("avformat_open_input error: %s\n", av_err2str(ret));
		return -1;
	}
	ret = avcodec_send_packet(codec_ctx, &packet);
	if (ret < 0){
		printf("avcodec_send_packet error: %s\n", av_err2str(ret));
		return -1;
	}
	ret = avcodec_receive_frame(codec_ctx, frame);
	if (ret < 0){
		printf("avcodec_send_packet error: %s\n", av_err2str(ret));
		return -1;
	}
	int size = 0;
	size = av_image_get_buffer_size(frame->format, frame->width, frame->height, 1);
	printf("size is %d\n", size);
	FILE *output_file = NULL;
	output_file = fopen("aa.yuv", "w+");
	if (NULL == output_file){
		perror("fopen");
		return -1;
	}
	if ((ret = fwrite(frame->data[0], 1, size, output_file)) < 0) {
		fprintf(stderr, "Failed to dump raw data.\n");
	}
	fclose(output_file);

	return 0;
}
