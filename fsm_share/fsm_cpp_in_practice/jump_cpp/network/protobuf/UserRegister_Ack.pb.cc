// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: UserRegister_Ack.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "UserRegister_Ack.pb.h"

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

const ::google::protobuf::Descriptor* UserRegister_Ack_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  UserRegister_Ack_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_UserRegister_5fAck_2eproto() {
  protobuf_AddDesc_UserRegister_5fAck_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "UserRegister_Ack.proto");
  GOOGLE_CHECK(file != NULL);
  UserRegister_Ack_descriptor_ = file->message_type(0);
  static const int UserRegister_Ack_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserRegister_Ack, msginfo_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserRegister_Ack, username_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserRegister_Ack, password_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserRegister_Ack, acktime_),
  };
  UserRegister_Ack_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      UserRegister_Ack_descriptor_,
      UserRegister_Ack::default_instance_,
      UserRegister_Ack_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserRegister_Ack, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(UserRegister_Ack, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(UserRegister_Ack));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_UserRegister_5fAck_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    UserRegister_Ack_descriptor_, &UserRegister_Ack::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_UserRegister_5fAck_2eproto() {
  delete UserRegister_Ack::default_instance_;
  delete UserRegister_Ack_reflection_;
}

void protobuf_AddDesc_UserRegister_5fAck_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026UserRegister_Ack.proto\022\010protobuf\"X\n\020Us"
    "erRegister_Ack\022\017\n\007msgInfo\030\001 \001(\t\022\020\n\010usern"
    "ame\030\002 \001(\t\022\020\n\010password\030\003 \001(\t\022\017\n\007acktime\030\004"
    " \002(\005", 124);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "UserRegister_Ack.proto", &protobuf_RegisterTypes);
  UserRegister_Ack::default_instance_ = new UserRegister_Ack();
  UserRegister_Ack::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_UserRegister_5fAck_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_UserRegister_5fAck_2eproto {
  StaticDescriptorInitializer_UserRegister_5fAck_2eproto() {
    protobuf_AddDesc_UserRegister_5fAck_2eproto();
  }
} static_descriptor_initializer_UserRegister_5fAck_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int UserRegister_Ack::kMsgInfoFieldNumber;
const int UserRegister_Ack::kUsernameFieldNumber;
const int UserRegister_Ack::kPasswordFieldNumber;
const int UserRegister_Ack::kAcktimeFieldNumber;
#endif  // !_MSC_VER

UserRegister_Ack::UserRegister_Ack()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void UserRegister_Ack::InitAsDefaultInstance() {
}

UserRegister_Ack::UserRegister_Ack(const UserRegister_Ack& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void UserRegister_Ack::SharedCtor() {
  _cached_size_ = 0;
  msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  username_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  password_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  acktime_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

UserRegister_Ack::~UserRegister_Ack() {
  SharedDtor();
}

void UserRegister_Ack::SharedDtor() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    delete msginfo_;
  }
  if (username_ != &::google::protobuf::internal::kEmptyString) {
    delete username_;
  }
  if (password_ != &::google::protobuf::internal::kEmptyString) {
    delete password_;
  }
  if (this != default_instance_) {
  }
}

void UserRegister_Ack::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* UserRegister_Ack::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return UserRegister_Ack_descriptor_;
}

const UserRegister_Ack& UserRegister_Ack::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_UserRegister_5fAck_2eproto();
  return *default_instance_;
}

UserRegister_Ack* UserRegister_Ack::default_instance_ = NULL;

UserRegister_Ack* UserRegister_Ack::New() const {
  return new UserRegister_Ack;
}

void UserRegister_Ack::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_msginfo()) {
      if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
        msginfo_->clear();
      }
    }
    if (has_username()) {
      if (username_ != &::google::protobuf::internal::kEmptyString) {
        username_->clear();
      }
    }
    if (has_password()) {
      if (password_ != &::google::protobuf::internal::kEmptyString) {
        password_->clear();
      }
    }
    acktime_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool UserRegister_Ack::MergePartialFromCodedStream(
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
        if (input->ExpectTag(18)) goto parse_username;
        break;
      }

      // optional string username = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_username:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_username()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->username().data(), this->username().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_password;
        break;
      }

      // optional string password = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_password:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_password()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->password().data(), this->password().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_acktime;
        break;
      }

      // required int32 acktime = 4;
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

void UserRegister_Ack::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string msgInfo = 1;
  if (has_msginfo()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->msginfo().data(), this->msginfo().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->msginfo(), output);
  }

  // optional string username = 2;
  if (has_username()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->username().data(), this->username().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->username(), output);
  }

  // optional string password = 3;
  if (has_password()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->password().data(), this->password().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->password(), output);
  }

  // required int32 acktime = 4;
  if (has_acktime()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->acktime(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* UserRegister_Ack::SerializeWithCachedSizesToArray(
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

  // optional string username = 2;
  if (has_username()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->username().data(), this->username().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->username(), target);
  }

  // optional string password = 3;
  if (has_password()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->password().data(), this->password().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->password(), target);
  }

  // required int32 acktime = 4;
  if (has_acktime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->acktime(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int UserRegister_Ack::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string msgInfo = 1;
    if (has_msginfo()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->msginfo());
    }

    // optional string username = 2;
    if (has_username()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->username());
    }

    // optional string password = 3;
    if (has_password()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->password());
    }

    // required int32 acktime = 4;
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

void UserRegister_Ack::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const UserRegister_Ack* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const UserRegister_Ack*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void UserRegister_Ack::MergeFrom(const UserRegister_Ack& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_msginfo()) {
      set_msginfo(from.msginfo());
    }
    if (from.has_username()) {
      set_username(from.username());
    }
    if (from.has_password()) {
      set_password(from.password());
    }
    if (from.has_acktime()) {
      set_acktime(from.acktime());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void UserRegister_Ack::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void UserRegister_Ack::CopyFrom(const UserRegister_Ack& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool UserRegister_Ack::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000008) != 0x00000008) return false;

  return true;
}

void UserRegister_Ack::Swap(UserRegister_Ack* other) {
  if (other != this) {
    std::swap(msginfo_, other->msginfo_);
    std::swap(username_, other->username_);
    std::swap(password_, other->password_);
    std::swap(acktime_, other->acktime_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata UserRegister_Ack::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = UserRegister_Ack_descriptor_;
  metadata.reflection = UserRegister_Ack_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

// @@protoc_insertion_point(global_scope)
