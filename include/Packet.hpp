#ifndef PACKET
#define PACKET

#include <rte_mbuf.h>
//think about getters, and descrution
class Packet {
 public:
   Packet() = default;
   Packet(uint8_t* data, size_t dataLen) : mData{data}, mDataLen{dataLen}, mbuf{nullptr} {}
   Packet(rte_mbuf* buf): mData{static_cast<uint8_t*>(buf->buf_addr)}, mDataLen{buf->data_len}, mbuf{buf} {}
   rte_mbuf* getMBuf() const { return mbuf;}
 private:
  uint8_t* mData;
  size_t mDataLen;
  rte_mbuf* mbuf; // only to be able to transfer pointer and free packet
};

#endif
