#pragma once
#include <stdint.h>
#include <cereal/types/common.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/functional.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/archives/binary.hpp>

namespace PacketCommon
{
//PackeHead
struct PacketHead {
	
public:
	PacketHead(uint32_t type) : type_(type) { }
	PacketHead() {}
	virtual uint32_t Type() const { return type_; }
	virtual void Type(uint32_t type) { type_ = type; }

private:
	friend class cereal::access;
	uint32_t type_;
	template<class Archive>
	void serialize(Archive &archive) {
		archive(CEREAL_NVP(type_));
	}
};
}
