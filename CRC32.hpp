#ifndef __CRC32_TABLR__
#define  __CRC32_TABLR__

#include <utility>
#include <cstdint>
#include <cstddef>
#include <cstdbool>

class CRC32 final {
	constexpr const static std::uint32_t __FLAG__ = 0b11111111111111111111111111111111;
	/*0xFFFFFFFF*/
	std::uint32_t __ans__ = __FLAG__;
public:

	inline void reset() noexcept(true) { __ans__ = __FLAG__; }
	inline std::uint32_t result() const noexcept(true) { return __ans__ ^ __FLAG__; }
	inline void append(const char * b, const char * e) noexcept(true) {
		append(reinterpret_cast<const unsigned char*>(b), reinterpret_cast<const unsigned char*>(e));
	}
	void append(const unsigned char *, const unsigned char *) noexcept(true);
	inline void append(const char * b, int s) noexcept(true) { append(b, b + s); }
	inline void append(const unsigned char * b, int s) noexcept(true) { append(b, b + s); }
};


#endif




