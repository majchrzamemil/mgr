#ifndef PACKET
#define PACKET

#include <rte_mbuf.h>
#include <rte_ether.h>

/*! Generic storage class for pakcet.*/
class Packet {
 public:
  /*!Socket Packet constructor */
  Packet(uint8_t* data, size_t dataLen) : mIpOffset{0u}, mData{data}, mDataLen{dataLen}, mbuf{nullptr} {}

  /*!Dpdk Packet constructor */
  Packet(rte_mbuf* buf): mIpOffset{mEthSize}, mData{static_cast<uint8_t*>(buf->buf_addr) + buf->data_off},
    mDataLen{buf->data_len - mEthSize},
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
  void setDataLen(size_t dataLen) {
    mDataLen = dataLen;
  }
  ~Packet() {}
 private:
  constexpr static size_t mEthSize{sizeof(ether_hdr)};
  const uint16_t mIpOffset;
  uint8_t* mData;
  size_t mDataLen;
  rte_mbuf* mbuf; // only to be able to transfer pointer and free packet
};

#endif
