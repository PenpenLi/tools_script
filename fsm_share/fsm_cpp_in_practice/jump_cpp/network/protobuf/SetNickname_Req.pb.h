// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SetNickname_Req.proto

#ifndef PROTOBUF_SetNickname_5fReq_2eproto__INCLUDED
#define PROTOBUF_SetNickname_5fReq_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_SetNickname_5fReq_2eproto();
void protobuf_AssignDesc_SetNickname_5fReq_2eproto();
void protobuf_ShutdownFile_SetNickname_5fReq_2eproto();

class SetNickname_Req;

// ===================================================================

class SetNickname_Req : public ::google::protobuf::Message {
 public:
  SetNickname_Req();
  virtual ~SetNickname_Req();

  SetNickname_Req(const SetNickname_Req& from);

  inline SetNickname_Req& operator=(const SetNickname_Req& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const SetNickname_Req& default_instance();

  void Swap(SetNickname_Req* other);

  // implements Message ----------------------------------------------

  SetNickname_Req* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SetNickname_Req& from);
  void MergeFrom(const SetNickname_Req& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string token = 1;
  inline bool has_token() const;
  inline void clear_token();
  static const int kTokenFieldNumber = 1;
  inline const ::std::string& token() const;
  inline void set_token(const ::std::string& value);
  inline void set_token(const char* value);
  inline void set_token(const char* value, size_t size);
  inline ::std::string* mutable_token();
  inline ::std::string* release_token();
  inline void set_allocated_token(::std::string* token);

  // required string nickname = 2;
  inline bool has_nickname() const;
  inline void clear_nickname();
  static const int kNicknameFieldNumber = 2;
  inline const ::std::string& nickname() const;
  inline void set_nickname(const ::std::string& value);
  inline void set_nickname(const char* value);
  inline void set_nickname(const char* value, size_t size);
  inline ::std::string* mutable_nickname();
  inline ::std::string* release_nickname();
  inline void set_allocated_nickname(::std::string* nickname);

  // @@protoc_insertion_point(class_scope:protobuf.SetNickname_Req)
 private:
  inline void set_has_token();
  inline void clear_has_token();
  inline void set_has_nickname();
  inline void clear_has_nickname();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* token_;
  ::std::string* nickname_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_SetNickname_5fReq_2eproto();
  friend void protobuf_AssignDesc_SetNickname_5fReq_2eproto();
  friend void protobuf_ShutdownFile_SetNickname_5fReq_2eproto();

  void InitAsDefaultInstance();
  static SetNickname_Req* default_instance_;
};
// ===================================================================


// ===================================================================

// SetNickname_Req

// required string token = 1;
inline bool SetNickname_Req::has_token() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SetNickname_Req::set_has_token() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SetNickname_Req::clear_has_token() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SetNickname_Req::clear_token() {
  if (token_ != &::google::protobuf::internal::kEmptyString) {
    token_->clear();
  }
  clear_has_token();
}
inline const ::std::string& SetNickname_Req::token() const {
  return *token_;
}
inline void SetNickname_Req::set_token(const ::std::string& value) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(value);
}
inline void SetNickname_Req::set_token(const char* value) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(value);
}
inline void SetNickname_Req::set_token(const char* value, size_t size) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SetNickname_Req::mutable_token() {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  return token_;
}
inline ::std::string* SetNickname_Req::release_token() {
  clear_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = token_;
    token_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void SetNickname_Req::set_allocated_token(::std::string* token) {
  if (token_ != &::google::protobuf::internal::kEmptyString) {
    delete token_;
  }
  if (token) {
    set_has_token();
    token_ = token;
  } else {
    clear_has_token();
    token_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string nickname = 2;
inline bool SetNickname_Req::has_nickname() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SetNickname_Req::set_has_nickname() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SetNickname_Req::clear_has_nickname() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SetNickname_Req::clear_nickname() {
  if (nickname_ != &::google::protobuf::internal::kEmptyString) {
    nickname_->clear();
  }
  clear_has_nickname();
}
inline const ::std::string& SetNickname_Req::nickname() const {
  return *nickname_;
}
inline void SetNickname_Req::set_nickname(const ::std::string& value) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(value);
}
inline void SetNickname_Req::set_nickname(const char* value) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(value);
}
inline void SetNickname_Req::set_nickname(const char* value, size_t size) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SetNickname_Req::mutable_nickname() {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  return nickname_;
}
inline ::std::string* SetNickname_Req::release_nickname() {
  clear_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = nickname_;
    nickname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void SetNickname_Req::set_allocated_nickname(::std::string* nickname) {
  if (nickname_ != &::google::protobuf::internal::kEmptyString) {
    delete nickname_;
  }
  if (nickname) {
    set_has_nickname();
    nickname_ = nickname;
  } else {
    clear_has_nickname();
    nickname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_SetNickname_5fReq_2eproto__INCLUDED
