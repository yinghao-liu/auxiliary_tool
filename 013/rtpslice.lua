-- new Proto
p_rtpslice = Proto.new("rtpslice", "part of rtp for test")

local f_proto = ProtoField.uint32("rtpslice.protocol", "Protocol", base.HEX)

p_rtpslice.fields = { f_proto }


function p_rtpslice.dissector(Tvb, Pinfo, TreeItem)
    Pinfo.cols.protocol:set("RTPSLICE")
    local subtree = TreeItem:add(p_rtpslice, Tvb(0,4))
    subtree:add(f_proto, Tvb(0,4))
end

-- add new protocol to dissector tab udp.port
udp_port_table = DissectorTable.get("udp.port")

udp_port_table:add(980, p_rtpslice)


