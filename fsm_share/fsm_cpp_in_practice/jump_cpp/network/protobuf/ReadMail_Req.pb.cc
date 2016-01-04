// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ReadMail_Req.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ReadMail_Req.pb.h"

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

const ::google::protobuf::Descriptor* ReadMail_Req_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ReadMail_Req_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ReadMail_5fReq_2eproto() {
  protobuf_AddDesc_ReadMail_5fReq_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ReadMail_Req.proto");
  GOOGLE_CHECK(file != NULL);
  ReadMail_Req_descriptor_ = file->message_type(0);
  static const int ReadMail_Req_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReadMail_Req, token_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReadMail_Req, mailid_),
  };
  ReadMail_Req_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ReadMail_Req_descriptor_,
      ReadMail_Req::default_instance_,
      ReadMail_Req_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReadMail_Req, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReadMail_Req, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ReadMail_Req));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ReadMail_5fReq_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ReadMail_Req_descriptor_, &ReadMail_Req::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ReadMail_5fReq_2eproto() {
  delete ReadMail_Req::default_instance_;
  delete ReadMail_Req_reflection_;
}

void protobuf_AddDesc_ReadMail_5fReq_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022ReadMail_Req.proto\022\010protobuf\"-\n\014ReadMa"
    "il_Req\022\r\n\005token\030\001 \002(\t\022\016\n\006mailId\030\002 \002(\005", 77);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ReadMail_Req.proto", &protobuf_RegisterTypes);
  ReadMail_Req::default_instance_ = new ReadMail_Req();
  ReadMail_Req::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ReadMail_5fReq_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ReadMail_5fReq_2eproto {
  StaticDescriptorInitializer_ReadMail_5fReq_2eproto() {
    protobuf_AddDesc_ReadMail_5fReq_2eproto();
  }
} static_descriptor_initializer_ReadMail_5fReq_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int ReadMail_Req::kTokenFieldNumber;
const int ReadMail_Req::kMailIdFieldNumber;
#endif  // !_MSC_VER

ReadMail_Req::ReadMail_Req()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ReadMail_Req::InitAsDefaultInstance() {
}

ReadMail_Req::ReadMail_Req(const ReadMail_Req& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ReadMail_Req::SharedCtor() {
  _cached_size_ = 0;
  token_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  mailid_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ReadMail_Req::~ReadMail_Req() {
  SharedDtor();
}

void ReadMail_Req::SharedDtor() {
  if (token_ != &::google::protobuf::internal::kEmptyString) {
    delete token_;
  }
  if (this != default_instance_) {
  }
}

void ReadMail_Req::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ReadMail_Req::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ReadMail_Req_descriptor_;
}

const ReadMail_Req& ReadMail_Req::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ReadMail_5fReq_2eproto();
  return *default_instance_;
}

ReadMail_Req* ReadMail_Req::default_instance_ = NULL;

ReadMail_Req* ReadMail_Req::New() const {
  return new ReadMail_Req;
}

void ReadMail_Req::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_token()) {
      if (token_ != &::google::protobuf::internal::kEmptyString) {
        token_->clear();
      }
    }
    mailid_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ReadMail_Req::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string token = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_token()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->token().data(), this->token().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_mailId;
        break;
      }

      // required int32 mailId = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_mailId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &mailid_)));
          set_has_mailid();
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

void ReadMail_Req::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string token = 1;
  if (has_token()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->token().data(), this->token().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->token(), output);
  }

  // required int32 mailId = 2;
  if (has_mailid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->mailid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ReadMail_Req::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string token = 1;
  if (has_token()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->token().data(), this->token().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->token(), target);
  }

  // required int32 mailId = 2;
  if (has_mailid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->mailid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ReadMail_Req::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string token = 1;
    if (has_token()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->token());
    }

    // required int32 mailId = 2;
    if (has_mailid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->mailid());
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

void ReadMail_Req::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ReadMail_Req* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ReadMail_Req*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ReadMail_Req::MergeFrom(const ReadMail_Req& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_token()) {
      set_token(from.token());
    }
    if (from.has_mailid()) {
      set_mailid(from.mailid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ReadMail_Req::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ReadMail_Req::CopyFrom(const ReadMail_Req& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ReadMail_Req::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void ReadMail_Req::Swap(ReadMail_Req* other) {
  if (other != this) {
    std::swap(token_, other->token_);
    std::swap(mailid_, other->mailid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ReadMail_Req::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ReadMail_Req_descriptor_;
  metadata.reflection = ReadMail_Req_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)
