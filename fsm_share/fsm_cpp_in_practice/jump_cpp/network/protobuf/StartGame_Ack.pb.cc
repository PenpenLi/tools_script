// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: StartGame_Ack.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "StartGame_Ack.pb.h"

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

const ::google::protobuf::Descriptor* StartGame_Ack_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  StartGame_Ack_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_StartGame_5fAck_2eproto() {
  protobuf_AddDesc_StartGame_5fAck_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "StartGame_Ack.proto");
  GOOGLE_CHECK(file != NULL);
  StartGame_Ack_descriptor_ = file->message_type(0);
  static const int StartGame_Ack_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(StartGame_Ack, msginfo_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(StartGame_Ack, helpcount_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(StartGame_Ack, acktime_),
  };
  StartGame_Ack_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      StartGame_Ack_descriptor_,
      StartGame_Ack::default_instance_,
      StartGame_Ack_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(StartGame_Ack, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(StartGame_Ack, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(StartGame_Ack));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_StartGame_5fAck_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    StartGame_Ack_descriptor_, &StartGame_Ack::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_StartGame_5fAck_2eproto() {
  delete StartGame_Ack::default_instance_;
  delete StartGame_Ack_reflection_;
}

void protobuf_AddDesc_StartGame_5fAck_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\023StartGame_Ack.proto\022\010protobuf\"D\n\rStart"
    "Game_Ack\022\017\n\007msgInfo\030\001 \001(\t\022\021\n\thelpCount\030\002"
    " \001(\005\022\017\n\007acktime\030\003 \002(\005", 101);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "StartGame_Ack.proto", &protobuf_RegisterTypes);
  StartGame_Ack::default_instance_ = new StartGame_Ack();
  StartGame_Ack::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_StartGame_5fAck_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_StartGame_5fAck_2eproto {
  StaticDescriptorInitializer_StartGame_5fAck_2eproto() {
    protobuf_AddDesc_StartGame_5fAck_2eproto();
  }
} static_descriptor_initializer_StartGame_5fAck_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int StartGame_Ack::kMsgInfoFieldNumber;
const int StartGame_Ack::kHelpCountFieldNumber;
const int StartGame_Ack::kAcktimeFieldNumber;
#endif  // !_MSC_VER

StartGame_Ack::StartGame_Ack()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void StartGame_Ack::InitAsDefaultInstance() {
}

StartGame_Ack::StartGame_Ack(const StartGame_Ack& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void StartGame_Ack::SharedCtor() {
  _cached_size_ = 0;
  msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  helpcount_ = 0;
  acktime_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

StartGame_Ack::~StartGame_Ack() {
  SharedDtor();
}

void StartGame_Ack::SharedDtor() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    delete msginfo_;
  }
  if (this != default_instance_) {
  }
}

void StartGame_Ack::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* StartGame_Ack::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return StartGame_Ack_descriptor_;
}

const StartGame_Ack& StartGame_Ack::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_StartGame_5fAck_2eproto();
  return *default_instance_;
}

StartGame_Ack* StartGame_Ack::default_instance_ = NULL;

StartGame_Ack* StartGame_Ack::New() const {
  return new StartGame_Ack;
}

void StartGame_Ack::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_msginfo()) {
      if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
        msginfo_->clear();
      }
    }
    helpcount_ = 0;
    acktime_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool StartGame_Ack::MergePartialFromCodedStream(
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
        if (input->ExpectTag(16)) goto parse_helpCount;
        break;
      }

      // optional int32 helpCount = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_helpCount:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &helpcount_)));
          set_has_helpcount();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_acktime;
        break;
      }

      // required int32 acktime = 3;
      case 3: {
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

void StartGame_Ack::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string msgInfo = 1;
  if (has_msginfo()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->msginfo().data(), this->msginfo().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->msginfo(), output);
  }

  // optional int32 helpCount = 2;
  if (has_helpcount()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->helpcount(), output);
  }

  // required int32 acktime = 3;
  if (has_acktime()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->acktime(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* StartGame_Ack::SerializeWithCachedSizesToArray(
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

  // optional int32 helpCount = 2;
  if (has_helpcount()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->helpcount(), target);
  }

  // required int32 acktime = 3;
  if (has_acktime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->acktime(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int StartGame_Ack::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string msgInfo = 1;
    if (has_msginfo()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->msginfo());
    }

    // optional int32 helpCount = 2;
    if (has_helpcount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->helpcount());
    }

    // required int32 acktime = 3;
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

void StartGame_Ack::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const StartGame_Ack* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const StartGame_Ack*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void StartGame_Ack::MergeFrom(const StartGame_Ack& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_msginfo()) {
      set_msginfo(from.msginfo());
    }
    if (from.has_helpcount()) {
      set_helpcount(from.helpcount());
    }
    if (from.has_acktime()) {
      set_acktime(from.acktime());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void StartGame_Ack::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void StartGame_Ack::CopyFrom(const StartGame_Ack& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool StartGame_Ack::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000004) != 0x00000004) return false;

  return true;
}

void StartGame_Ack::Swap(StartGame_Ack* other) {
  if (other != this) {
    std::swap(msginfo_, other->msginfo_);
    std::swap(helpcount_, other->helpcount_);
    std::swap(acktime_, other->acktime_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata StartGame_Ack::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = StartGame_Ack_descriptor_;
  metadata.reflection = StartGame_Ack_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)
