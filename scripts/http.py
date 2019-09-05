from trex_stl_lib.api import *

class STLS1(object):

    def create_stream (self):
        return STLStream(
            packet =
                    STLPktBuilder(
                        pkt = Ether(src="e0:cb:4e:47:34:c0", dst="28:d2:44:07:d5:ac")/IP(src="192.168.1.2",dst="192.168.1.1")/TCP(dport=8080,sport=1234, flags="PA")/("GET /index.html HTTP/1.0 \n\n" )
                    ),
             mode = STLTXCont())

    def get_streams (self, direction = 0, **kwargs):
        # create 1 stream
        return [ self.create_stream() ]


# dynamic load - used for trex console or simulator
def register():
    return STLS1()



