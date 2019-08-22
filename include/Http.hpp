#include "http_hdr.hpp"
class Http {
  public:
    Http(http_hdr* hdr): mHdr{hdr} {}

  private:
    http_hdr* mHdr;
};
