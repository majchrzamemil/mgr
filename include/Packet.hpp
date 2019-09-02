#ifndef PACKET
#define PACKET

#include <rte_mbuf.h>
#include <rte_ether.h>

class Packet {
 public:
  Packet(uint8_t* data, size_t dataLen) : mIpOffset{0u}, mData{data}, mDataLen{dataLen}, mbuf{nullptr} {}
  Packet(rte_mbuf* buf): mIpOffset{mEthSize}, mData{static_cast<uint8_t*>(buf->buf_addr) + buf->data_off},
    mDataLen{buf->data_len},
    mbuf{buf} {}
  rte_mbuf* getMBuf() const {
    return mbuf;
  }
  void freeData() {
    delete mData;
  }
  uint8_t* getData() const {
    return mData + mIpOffset;
  }
  size_t getDataLen() const {
    return mDataLen;
  }
  ~Packet() {}
 private:
  constexpr static uint16_t mEthSize{sizeof(ether_hdr)};
  const uint16_t mIpOffset;
  uint8_t* mData;
  size_t mDataLen;
  rte_mbuf* mbuf; // only to be able to transfer pointer and free packet
};

#endif
