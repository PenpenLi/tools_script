// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ItemAdd_Ack.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ItemAdd_Ack.pb.h"

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

const ::google::protobuf::Descriptor* ItemAdd_Ack_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ItemAdd_Ack_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ItemAdd_5fAck_2eproto() {
  protobuf_AddDesc_ItemAdd_5fAck_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ItemAdd_Ack.proto");
  GOOGLE_CHECK(file != NULL);
  ItemAdd_Ack_descriptor_ = file->message_type(0);
  static const int ItemAdd_Ack_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemAdd_Ack, msginfo_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemAdd_Ack, itemid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemAdd_Ack, count_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemAdd_Ack, acktime_),
  };
  ItemAdd_Ack_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ItemAdd_Ack_descriptor_,
      ItemAdd_Ack::default_instance_,
      ItemAdd_Ack_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemAdd_Ack, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemAdd_Ack, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ItemAdd_Ack));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ItemAdd_5fAck_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ItemAdd_Ack_descriptor_, &ItemAdd_Ack::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ItemAdd_5fAck_2eproto() {
  delete ItemAdd_Ack::default_instance_;
  delete ItemAdd_Ack_reflection_;
}

void protobuf_AddDesc_ItemAdd_5fAck_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021ItemAdd_Ack.proto\022\010protobuf\"N\n\013ItemAdd"
    "_Ack\022\017\n\007msgInfo\030\001 \001(\t\022\016\n\006itemid\030\002 \001(\005\022\r\n"
    "\005count\030\003 \001(\005\022\017\n\007acktime\030\004 \001(\005", 109);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ItemAdd_Ack.proto", &protobuf_RegisterTypes);
  ItemAdd_Ack::default_instance_ = new ItemAdd_Ack();
  ItemAdd_Ack::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ItemAdd_5fAck_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ItemAdd_5fAck_2eproto {
  StaticDescriptorInitializer_ItemAdd_5fAck_2eproto() {
    protobuf_AddDesc_ItemAdd_5fAck_2eproto();
  }
} static_descriptor_initializer_ItemAdd_5fAck_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int ItemAdd_Ack::kMsgInfoFieldNumber;
const int ItemAdd_Ack::kItemidFieldNumber;
const int ItemAdd_Ack::kCountFieldNumber;
const int ItemAdd_Ack::kAcktimeFieldNumber;
#endif  // !_MSC_VER

ItemAdd_Ack::ItemAdd_Ack()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ItemAdd_Ack::InitAsDefaultInstance() {
}

ItemAdd_Ack::ItemAdd_Ack(const ItemAdd_Ack& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ItemAdd_Ack::SharedCtor() {
  _cached_size_ = 0;
  msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  itemid_ = 0;
  count_ = 0;
  acktime_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ItemAdd_Ack::~ItemAdd_Ack() {
  SharedDtor();
}

void ItemAdd_Ack::SharedDtor() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    delete msginfo_;
  }
  if (this != default_instance_) {
  }
}

void ItemAdd_Ack::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ItemAdd_Ack::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ItemAdd_Ack_descriptor_;
}

const ItemAdd_Ack& ItemAdd_Ack::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ItemAdd_5fAck_2eproto();
  return *default_instance_;
}

ItemAdd_Ack* ItemAdd_Ack::default_instance_ = NULL;

ItemAdd_Ack* ItemAdd_Ack::New() const {
  return new ItemAdd_Ack;
}

void ItemAdd_Ack::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_msginfo()) {
      if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
        msginfo_->clear();
      }
    }
    itemid_ = 0;
    count_ = 0;
    acktime_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ItemAdd_Ack::MergePartialFromCodedStream(
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
        if (input->ExpectTag(16)) goto parse_itemid;
        break;
      }

      // optional int32 itemid = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_itemid:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &itemid_)));
          set_has_itemid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_count;
        break;
      }

      // optional int32 count = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_count:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &count_)));
          set_has_count();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_acktime;
        break;
      }

      // optional int32 acktime = 4;
      case 4: {
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

void ItemAdd_Ack::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string msgInfo = 1;
  if (has_msginfo()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->msginfo().data(), this->msginfo().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->msginfo(), output);
  }

  // optional int32 itemid = 2;
  if (has_itemid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->itemid(), output);
  }

  // optional int32 count = 3;
  if (has_count()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->count(), output);
  }

  // optional int32 acktime = 4;
  if (has_acktime()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->acktime(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ItemAdd_Ack::SerializeWithCachedSizesToArray(
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

  // optional int32 itemid = 2;
  if (has_itemid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->itemid(), target);
  }

  // optional int32 count = 3;
  if (has_count()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->count(), target);
  }

  // optional int32 acktime = 4;
  if (has_acktime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->acktime(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ItemAdd_Ack::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string msgInfo = 1;
    if (has_msginfo()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->msginfo());
    }

    // optional int32 itemid = 2;
    if (has_itemid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->itemid());
    }

    // optional int32 count = 3;
    if (has_count()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->count());
    }

    // optional int32 acktime = 4;
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

void ItemAdd_Ack::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ItemAdd_Ack* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ItemAdd_Ack*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ItemAdd_Ack::MergeFrom(const ItemAdd_Ack& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_msginfo()) {
      set_msginfo(from.msginfo());
    }
    if (from.has_itemid()) {
      set_itemid(from.itemid());
    }
    if (from.has_count()) {
      set_count(from.count());
    }
    if (from.has_acktime()) {
      set_acktime(from.acktime());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ItemAdd_Ack::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ItemAdd_Ack::CopyFrom(const ItemAdd_Ack& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ItemAdd_Ack::IsInitialized() const {

  return true;
}

void ItemAdd_Ack::Swap(ItemAdd_Ack* other) {
  if (other != this) {
    std::swap(msginfo_, other->msginfo_);
    std::swap(itemid_, other->itemid_);
    std::swap(count_, other->count_);
    std::swap(acktime_, other->acktime_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ItemAdd_Ack::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ItemAdd_Ack_descriptor_;
  metadata.reflection = ItemAdd_Ack_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)
