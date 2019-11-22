from trex_stl_lib.api import *

class STLS1(object):

    def create_stream (self):
        return STLStream(
            packet =
                    STLPktBuilder(
                        pkt = Ether(src="24:6e:96:28:08:d0", dst="24:6e:96:14:a6:c8")/IP(src="192.168.1.2",dst="192.168.1.1")/TCP(dport=8080,sport=1234, flags="PA")/("GET /index.html HTTP/1.0 \n\n" )
                    ),
             mode = STLTXCont())

    def get_streams (self, direction = 0, **kwargs):
        # create 1 stream
        return [ self.create_stream() ]


# dynamic load - used for trex console or simulator
def register():
    return STLS1()



