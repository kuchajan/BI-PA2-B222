#include "CArt.hpp"

CArt::CArt(const CImage &image, const CCharset &charset, const vector<shared_ptr<CFilter>> &filters) : m_result(getResult(getGrayScale(image, filters), charset)) {}