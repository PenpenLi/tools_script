// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BuyRole_Ack.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "BuyRole_Ack.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace protobuf {

namespace {

const ::google::protobuf::Descriptor* BuyRole_Ack_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BuyRole_Ack_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_BuyRole_5fAck_2eproto() {
  protobuf_AddDesc_BuyRole_5fAck_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "BuyRole_Ack.proto");
  GOOGLE_CHECK(file != NULL);
  BuyRole_Ack_descriptor_ = file->message_type(0);
  static const int BuyRole_Ack_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BuyRole_Ack, msginfo_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BuyRole_Ack, roletype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BuyRole_Ack, coin_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BuyRole_Ack, diamond_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BuyRole_Ack, acktime_),
  };
  BuyRole_Ack_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BuyRole_Ack_descriptor_,
      BuyRole_Ack::default_instance_,
      BuyRole_Ack_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BuyRole_Ack, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BuyRole_Ack, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BuyRole_Ack));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_BuyRole_5fAck_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BuyRole_Ack_descriptor_, &BuyRole_Ack::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_BuyRole_5fAck_2eproto() {
  delete BuyRole_Ack::default_instance_;
  delete BuyRole_Ack_reflection_;
}

void protobuf_AddDesc_BuyRole_5fAck_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021BuyRole_Ack.proto\022\010protobuf\"`\n\013BuyRole"
    "_Ack\022\017\n\007msgInfo\030\001 \001(\t\022\020\n\010roleType\030\002 \001(\005\022"
    "\014\n\004coin\030\003 \001(\005\022\017\n\007diamond\030\004 \001(\005\022\017\n\007acktim"
    "e\030\005 \001(\005", 127);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "BuyRole_Ack.proto", &protobuf_RegisterTypes);
  BuyRole_Ack::default_instance_ = new BuyRole_Ack();
  BuyRole_Ack::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_BuyRole_5fAck_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_BuyRole_5fAck_2eproto {
  StaticDescriptorInitializer_BuyRole_5fAck_2eproto() {
    protobuf_AddDesc_BuyRole_5fAck_2eproto();
  }
} static_descriptor_initializer_BuyRole_5fAck_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int BuyRole_Ack::kMsgInfoFieldNumber;
const int BuyRole_Ack::kRoleTypeFieldNumber;
const int BuyRole_Ack::kCoinFieldNumber;
const int BuyRole_Ack::kDiamondFieldNumber;
const int BuyRole_Ack::kAcktimeFieldNumber;
#endif  // !_MSC_VER

BuyRole_Ack::BuyRole_Ack()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BuyRole_Ack::InitAsDefaultInstance() {
}

BuyRole_Ack::BuyRole_Ack(const BuyRole_Ack& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void BuyRole_Ack::SharedCtor() {
  _cached_size_ = 0;
  msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  roletype_ = 0;
  coin_ = 0;
  diamond_ = 0;
  acktime_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BuyRole_Ack::~BuyRole_Ack() {
  SharedDtor();
}

void BuyRole_Ack::SharedDtor() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    delete msginfo_;
  }
  if (this != default_instance_) {
  }
}

void BuyRole_Ack::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* BuyRole_Ack::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BuyRole_Ack_descriptor_;
}

const BuyRole_Ack& BuyRole_Ack::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_BuyRole_5fAck_2eproto();
  return *default_instance_;
}

BuyRole_Ack* BuyRole_Ack::default_instance_ = NULL;

BuyRole_Ack* BuyRole_Ack::New() const {
  return new BuyRole_Ack;
}

void BuyRole_Ack::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_msginfo()) {
      if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
        msginfo_->clear();
      }
    }
    roletype_ = 0;
    coin_ = 0;
    diamond_ = 0;
    acktime_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool BuyRole_Ack::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string msgInfo = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_msginfo()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->msginfo().data(), this->msginfo().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_roleType;
        break;
      }

      // optional int32 roleType = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_roleType:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &roletype_)));
          set_has_roletype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_coin;
        break;
      }

      // optional int32 coin = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_coin:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &coin_)));
          set_has_coin();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_diamond;
        break;
      }

      // optional int32 diamond = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_diamond:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &diamond_)));
          set_has_diamond();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_acktime;
        break;
      }

      // optional int32 acktime = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_acktime:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &acktime_)));
          set_has_acktime();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void BuyRole_Ack::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string msgInfo = 1;
  if (has_msginfo()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->msginfo().data(), this->msginfo().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->msginfo(), output);
  }

  // optional int32 roleType = 2;
  if (has_roletype()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->roletype(), output);
  }

  // optional int32 coin = 3;
  if (has_coin()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->coin(), output);
  }

  // optional int32 diamond = 4;
  if (has_diamond()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->diamond(), output);
  }

  // optional int32 acktime = 5;
  if (has_acktime()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(5, this->acktime(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BuyRole_Ack::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string msgInfo = 1;
  if (has_msginfo()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->msginfo().data(), this->msginfo().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->msginfo(), target);
  }

  // optional int32 roleType = 2;
  if (has_roletype()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->roletype(), target);
  }

  // optional int32 coin = 3;
  if (has_coin()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->coin(), target);
  }

  // optional int32 diamond = 4;
  if (has_diamond()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->diamond(), target);
  }

  // optional int32 acktime = 5;
  if (has_acktime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(5, this->acktime(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int BuyRole_Ack::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string msgInfo = 1;
    if (has_msginfo()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->msginfo());
    }

    // optional int32 roleType = 2;
    if (has_roletype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->roletype());
    }

    // optional int32 coin = 3;
    if (has_coin()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->coin());
    }

    // optional int32 diamond = 4;
    if (has_diamond()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->diamond());
    }

    // optional int32 acktime = 5;
    if (has_acktime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->acktime());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BuyRole_Ack::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BuyRole_Ack* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BuyRole_Ack*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BuyRole_Ack::MergeFrom(const BuyRole_Ack& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_msginfo()) {
      set_msginfo(from.msginfo());
    }
    if (from.has_roletype()) {
      set_roletype(from.roletype());
    }
    if (from.has_coin()) {
      set_coin(from.coin());
    }
    if (from.has_diamond()) {
      set_diamond(from.diamond());
    }
    if (from.has_acktime()) {
      set_acktime(from.acktime());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BuyRole_Ack::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BuyRole_Ack::CopyFrom(const BuyRole_Ack& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BuyRole_Ack::IsInitialized() const {

  return true;
}

void BuyRole_Ack::Swap(BuyRole_Ack* other) {
  if (other != this) {
    std::swap(msginfo_, other->msginfo_);
    std::swap(roletype_, other->roletype_);
    std::swap(coin_, other->coin_);
    std::swap(diamond_, other->diamond_);
    std::swap(acktime_, other->acktime_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BuyRole_Ack::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BuyRole_Ack_descriptor_;
  metadata.reflection = BuyRole_Ack_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)
