// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Result_Req.proto

#ifndef PROTOBUF_Result_5fReq_2eproto__INCLUDED
#define PROTOBUF_Result_5fReq_2eproto__INCLUDED

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
void  protobuf_AddDesc_Result_5fReq_2eproto();
void protobuf_AssignDesc_Result_5fReq_2eproto();
void protobuf_ShutdownFile_Result_5fReq_2eproto();

class Result_Req;

// ===================================================================

class Result_Req : public ::google::protobuf::Message {
 public:
  Result_Req();
  virtual ~Result_Req();

  Result_Req(const Result_Req& from);

  inline Result_Req& operator=(const Result_Req& from) {
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
  static const Result_Req& default_instance();

  void Swap(Result_Req* other);

  // implements Message ----------------------------------------------

  Result_Req* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Result_Req& from);
  void MergeFrom(const Result_Req& from);
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

  // required int32 score = 2;
  inline bool has_score() const;
  inline void clear_score();
  static const int kScoreFieldNumber = 2;
  inline ::google::protobuf::int32 score() const;
  inline void set_score(::google::protobuf::int32 value);

  // required int32 coin = 3;
  inline bool has_coin() const;
  inline void clear_coin();
  static const int kCoinFieldNumber = 3;
  inline ::google::protobuf::int32 coin() const;
  inline void set_coin(::google::protobuf::int32 value);

  // required int32 jumpheiht = 4;
  inline bool has_jumpheiht() const;
  inline void clear_jumpheiht();
  static const int kJumpheihtFieldNumber = 4;
  inline ::google::protobuf::int32 jumpheiht() const;
  inline void set_jumpheiht(::google::protobuf::int32 value);

  // required int32 killmonster = 5;
  inline bool has_killmonster() const;
  inline void clear_killmonster();
  static const int kKillmonsterFieldNumber = 5;
  inline ::google::protobuf::int32 killmonster() const;
  inline void set_killmonster(::google::protobuf::int32 value);

  // required int32 star = 6;
  inline bool has_star() const;
  inline void clear_star();
  static const int kStarFieldNumber = 6;
  inline ::google::protobuf::int32 star() const;
  inline void set_star(::google::protobuf::int32 value);

  // required int32 revive = 7;
  inline bool has_revive() const;
  inline void clear_revive();
  static const int kReviveFieldNumber = 7;
  inline ::google::protobuf::int32 revive() const;
  inline void set_revive(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf.Result_Req)
 private:
  inline void set_has_token();
  inline void clear_has_token();
  inline void set_has_score();
  inline void clear_has_score();
  inline void set_has_coin();
  inline void clear_has_coin();
  inline void set_has_jumpheiht();
  inline void clear_has_jumpheiht();
  inline void set_has_killmonster();
  inline void clear_has_killmonster();
  inline void set_has_star();
  inline void clear_has_star();
  inline void set_has_revive();
  inline void clear_has_revive();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* token_;
  ::google::protobuf::int32 score_;
  ::google::protobuf::int32 coin_;
  ::google::protobuf::int32 jumpheiht_;
  ::google::protobuf::int32 killmonster_;
  ::google::protobuf::int32 star_;
  ::google::protobuf::int32 revive_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];

  friend void  protobuf_AddDesc_Result_5fReq_2eproto();
  friend void protobuf_AssignDesc_Result_5fReq_2eproto();
  friend void protobuf_ShutdownFile_Result_5fReq_2eproto();

  void InitAsDefaultInstance();
  static Result_Req* default_instance_;
};
// ===================================================================


// ===================================================================

// Result_Req

// required string token = 1;
inline bool Result_Req::has_token() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Result_Req::set_has_token() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Result_Req::clear_has_token() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Result_Req::clear_token() {
  if (token_ != &::google::protobuf::internal::kEmptyString) {
    token_->clear();
  }
  clear_has_token();
}
inline const ::std::string& Result_Req::token() const {
  return *token_;
}
inline void Result_Req::set_token(const ::std::string& value) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(value);
}
inline void Result_Req::set_token(const char* value) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(value);
}
inline void Result_Req::set_token(const char* value, size_t size) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  token_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Result_Req::mutable_token() {
  set_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    token_ = new ::std::string;
  }
  return token_;
}
inline ::std::string* Result_Req::release_token() {
  clear_has_token();
  if (token_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = token_;
    token_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Result_Req::set_allocated_token(::std::string* token) {
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

// required int32 score = 2;
inline bool Result_Req::has_score() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Result_Req::set_has_score() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Result_Req::clear_has_score() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Result_Req::clear_score() {
  score_ = 0;
  clear_has_score();
}
inline ::google::protobuf::int32 Result_Req::score() const {
  return score_;
}
inline void Result_Req::set_score(::google::protobuf::int32 value) {
  set_has_score();
  score_ = value;
}

// required int32 coin = 3;
inline bool Result_Req::has_coin() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Result_Req::set_has_coin() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Result_Req::clear_has_coin() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Result_Req::clear_coin() {
  coin_ = 0;
  clear_has_coin();
}
inline ::google::protobuf::int32 Result_Req::coin() const {
  return coin_;
}
inline void Result_Req::set_coin(::google::protobuf::int32 value) {
  set_has_coin();
  coin_ = value;
}

// required int32 jumpheiht = 4;
inline bool Result_Req::has_jumpheiht() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Result_Req::set_has_jumpheiht() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Result_Req::clear_has_jumpheiht() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Result_Req::clear_jumpheiht() {
  jumpheiht_ = 0;
  clear_has_jumpheiht();
}
inline ::google::protobuf::int32 Result_Req::jumpheiht() const {
  return jumpheiht_;
}
inline void Result_Req::set_jumpheiht(::google::protobuf::int32 value) {
  set_has_jumpheiht();
  jumpheiht_ = value;
}

// required int32 killmonster = 5;
inline bool Result_Req::has_killmonster() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Result_Req::set_has_killmonster() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Result_Req::clear_has_killmonster() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Result_Req::clear_killmonster() {
  killmonster_ = 0;
  clear_has_killmonster();
}
inline ::google::protobuf::int32 Result_Req::killmonster() const {
  return killmonster_;
}
inline void Result_Req::set_killmonster(::google::protobuf::int32 value) {
  set_has_killmonster();
  killmonster_ = value;
}

// required int32 star = 6;
inline bool Result_Req::has_star() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void Result_Req::set_has_star() {
  _has_bits_[0] |= 0x00000020u;
}
inline void Result_Req::clear_has_star() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void Result_Req::clear_star() {
  star_ = 0;
  clear_has_star();
}
inline ::google::protobuf::int32 Result_Req::star() const {
  return star_;
}
inline void Result_Req::set_star(::google::protobuf::int32 value) {
  set_has_star();
  star_ = value;
}

// required int32 revive = 7;
inline bool Result_Req::has_revive() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void Result_Req::set_has_revive() {
  _has_bits_[0] |= 0x00000040u;
}
inline void Result_Req::clear_has_revive() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void Result_Req::clear_revive() {
  revive_ = 0;
  clear_has_revive();
}
inline ::google::protobuf::int32 Result_Req::revive() const {
  return revive_;
}
inline void Result_Req::set_revive(::google::protobuf::int32 value) {
  set_has_revive();
  revive_ = value;
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

#endif  // PROTOBUF_Result_5fReq_2eproto__INCLUDED
