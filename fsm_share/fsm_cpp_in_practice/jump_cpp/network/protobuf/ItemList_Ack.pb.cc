// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ItemList_Ack.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ItemList_Ack.pb.h"

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

const ::google::protobuf::Descriptor* ItemList_Ack_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ItemList_Ack_reflection_ = NULL;
const ::google::protobuf::Descriptor* ItemList_Ack_ItemList_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ItemList_Ack_ItemList_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ItemList_5fAck_2eproto() {
  protobuf_AddDesc_ItemList_5fAck_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ItemList_Ack.proto");
  GOOGLE_CHECK(file != NULL);
  ItemList_Ack_descriptor_ = file->message_type(0);
  static const int ItemList_Ack_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack, msginfo_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack, itemlist_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack, itemid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack, acktime_),
  };
  ItemList_Ack_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ItemList_Ack_descriptor_,
      ItemList_Ack::default_instance_,
      ItemList_Ack_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ItemList_Ack));
  ItemList_Ack_ItemList_descriptor_ = ItemList_Ack_descriptor_->nested_type(0);
  static const int ItemList_Ack_ItemList_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack_ItemList, itemid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack_ItemList, count_),
  };
  ItemList_Ack_ItemList_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ItemList_Ack_ItemList_descriptor_,
      ItemList_Ack_ItemList::default_instance_,
      ItemList_Ack_ItemList_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack_ItemList, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ItemList_Ack_ItemList, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ItemList_Ack_ItemList));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ItemList_5fAck_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ItemList_Ack_descriptor_, &ItemList_Ack::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ItemList_Ack_ItemList_descriptor_, &ItemList_Ack_ItemList::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ItemList_5fAck_2eproto() {
  delete ItemList_Ack::default_instance_;
  delete ItemList_Ack_reflection_;
  delete ItemList_Ack_ItemList::default_instance_;
  delete ItemList_Ack_ItemList_reflection_;
}

void protobuf_AddDesc_ItemList_5fAck_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022ItemList_Ack.proto\022\010protobuf\"\236\001\n\014ItemL"
    "ist_Ack\022\017\n\007msgInfo\030\001 \001(\t\0221\n\010itemlist\030\002 \003"
    "(\0132\037.protobuf.ItemList_Ack.ItemList\022\016\n\006I"
    "temId\030\003 \001(\005\022\017\n\007acktime\030\004 \001(\005\032)\n\010ItemList"
    "\022\016\n\006itemid\030\001 \001(\005\022\r\n\005count\030\002 \001(\005", 191);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ItemList_Ack.proto", &protobuf_RegisterTypes);
  ItemList_Ack::default_instance_ = new ItemList_Ack();
  ItemList_Ack_ItemList::default_instance_ = new ItemList_Ack_ItemList();
  ItemList_Ack::default_instance_->InitAsDefaultInstance();
  ItemList_Ack_ItemList::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ItemList_5fAck_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ItemList_5fAck_2eproto {
  StaticDescriptorInitializer_ItemList_5fAck_2eproto() {
    protobuf_AddDesc_ItemList_5fAck_2eproto();
  }
} static_descriptor_initializer_ItemList_5fAck_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int ItemList_Ack_ItemList::kItemidFieldNumber;
const int ItemList_Ack_ItemList::kCountFieldNumber;
#endif  // !_MSC_VER

ItemList_Ack_ItemList::ItemList_Ack_ItemList()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ItemList_Ack_ItemList::InitAsDefaultInstance() {
}

ItemList_Ack_ItemList::ItemList_Ack_ItemList(const ItemList_Ack_ItemList& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ItemList_Ack_ItemList::SharedCtor() {
  _cached_size_ = 0;
  itemid_ = 0;
  count_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ItemList_Ack_ItemList::~ItemList_Ack_ItemList() {
  SharedDtor();
}

void ItemList_Ack_ItemList::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ItemList_Ack_ItemList::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ItemList_Ack_ItemList::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ItemList_Ack_ItemList_descriptor_;
}

const ItemList_Ack_ItemList& ItemList_Ack_ItemList::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ItemList_5fAck_2eproto();
  return *default_instance_;
}

ItemList_Ack_ItemList* ItemList_Ack_ItemList::default_instance_ = NULL;

ItemList_Ack_ItemList* ItemList_Ack_ItemList::New() const {
  return new ItemList_Ack_ItemList;
}

void ItemList_Ack_ItemList::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    itemid_ = 0;
    count_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ItemList_Ack_ItemList::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 itemid = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &itemid_)));
          set_has_itemid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_count;
        break;
      }

      // optional int32 count = 2;
      case 2: {
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

void ItemList_Ack_ItemList::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 itemid = 1;
  if (has_itemid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->itemid(), output);
  }

  // optional int32 count = 2;
  if (has_count()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->count(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ItemList_Ack_ItemList::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 itemid = 1;
  if (has_itemid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->itemid(), target);
  }

  // optional int32 count = 2;
  if (has_count()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->count(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ItemList_Ack_ItemList::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 itemid = 1;
    if (has_itemid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->itemid());
    }

    // optional int32 count = 2;
    if (has_count()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->count());
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

void ItemList_Ack_ItemList::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ItemList_Ack_ItemList* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ItemList_Ack_ItemList*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ItemList_Ack_ItemList::MergeFrom(const ItemList_Ack_ItemList& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_itemid()) {
      set_itemid(from.itemid());
    }
    if (from.has_count()) {
      set_count(from.count());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ItemList_Ack_ItemList::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ItemList_Ack_ItemList::CopyFrom(const ItemList_Ack_ItemList& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ItemList_Ack_ItemList::IsInitialized() const {

  return true;
}

void ItemList_Ack_ItemList::Swap(ItemList_Ack_ItemList* other) {
  if (other != this) {
    std::swap(itemid_, other->itemid_);
    std::swap(count_, other->count_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ItemList_Ack_ItemList::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ItemList_Ack_ItemList_descriptor_;
  metadata.reflection = ItemList_Ack_ItemList_reflection_;
  return metadata;
}


// -------------------------------------------------------------------

#ifndef _MSC_VER
const int ItemList_Ack::kMsgInfoFieldNumber;
const int ItemList_Ack::kItemlistFieldNumber;
const int ItemList_Ack::kItemIdFieldNumber;
const int ItemList_Ack::kAcktimeFieldNumber;
#endif  // !_MSC_VER

ItemList_Ack::ItemList_Ack()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ItemList_Ack::InitAsDefaultInstance() {
}

ItemList_Ack::ItemList_Ack(const ItemList_Ack& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ItemList_Ack::SharedCtor() {
  _cached_size_ = 0;
  msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  itemid_ = 0;
  acktime_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ItemList_Ack::~ItemList_Ack() {
  SharedDtor();
}

void ItemList_Ack::SharedDtor() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    delete msginfo_;
  }
  if (this != default_instance_) {
  }
}

void ItemList_Ack::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ItemList_Ack::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ItemList_Ack_descriptor_;
}

const ItemList_Ack& ItemList_Ack::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ItemList_5fAck_2eproto();
  return *default_instance_;
}

ItemList_Ack* ItemList_Ack::default_instance_ = NULL;

ItemList_Ack* ItemList_Ack::New() const {
  return new ItemList_Ack;
}

void ItemList_Ack::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_msginfo()) {
      if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
        msginfo_->clear();
      }
    }
    itemid_ = 0;
    acktime_ = 0;
  }
  itemlist_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ItemList_Ack::MergePartialFromCodedStream(
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
        if (input->ExpectTag(18)) goto parse_itemlist;
        break;
      }

      // repeated .protobuf.ItemList_Ack.ItemList itemlist = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_itemlist:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_itemlist()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_itemlist;
        if (input->ExpectTag(24)) goto parse_ItemId;
        break;
      }

      // optional int32 ItemId = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_ItemId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &itemid_)));
          set_has_itemid();
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

void ItemList_Ack::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string msgInfo = 1;
  if (has_msginfo()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->msginfo().data(), this->msginfo().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->msginfo(), output);
  }

  // repeated .protobuf.ItemList_Ack.ItemList itemlist = 2;
  for (int i = 0; i < this->itemlist_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->itemlist(i), output);
  }

  // optional int32 ItemId = 3;
  if (has_itemid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->itemid(), output);
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

::google::protobuf::uint8* ItemList_Ack::SerializeWithCachedSizesToArray(
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

  // repeated .protobuf.ItemList_Ack.ItemList itemlist = 2;
  for (int i = 0; i < this->itemlist_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->itemlist(i), target);
  }

  // optional int32 ItemId = 3;
  if (has_itemid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->itemid(), target);
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

int ItemList_Ack::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string msgInfo = 1;
    if (has_msginfo()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->msginfo());
    }

    // optional int32 ItemId = 3;
    if (has_itemid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->itemid());
    }

    // optional int32 acktime = 4;
    if (has_acktime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->acktime());
    }

  }
  // repeated .protobuf.ItemList_Ack.ItemList itemlist = 2;
  total_size += 1 * this->itemlist_size();
  for (int i = 0; i < this->itemlist_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->itemlist(i));
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

void ItemList_Ack::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ItemList_Ack* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ItemList_Ack*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ItemList_Ack::MergeFrom(const ItemList_Ack& from) {
  GOOGLE_CHECK_NE(&from, this);
  itemlist_.MergeFrom(from.itemlist_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_msginfo()) {
      set_msginfo(from.msginfo());
    }
    if (from.has_itemid()) {
      set_itemid(from.itemid());
    }
    if (from.has_acktime()) {
      set_acktime(from.acktime());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ItemList_Ack::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ItemList_Ack::CopyFrom(const ItemList_Ack& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ItemList_Ack::IsInitialized() const {

  return true;
}

void ItemList_Ack::Swap(ItemList_Ack* other) {
  if (other != this) {
    std::swap(msginfo_, other->msginfo_);
    itemlist_.Swap(&other->itemlist_);
    std::swap(itemid_, other->itemid_);
    std::swap(acktime_, other->acktime_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ItemList_Ack::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ItemList_Ack_descriptor_;
  metadata.reflection = ItemList_Ack_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)
