// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FriendHelpList_Ack.proto

#ifndef PROTOBUF_FriendHelpList_5fAck_2eproto__INCLUDED
#define PROTOBUF_FriendHelpList_5fAck_2eproto__INCLUDED

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
void  protobuf_AddDesc_FriendHelpList_5fAck_2eproto();
void protobuf_AssignDesc_FriendHelpList_5fAck_2eproto();
void protobuf_ShutdownFile_FriendHelpList_5fAck_2eproto();

class FriendHelpList_Ack;
class FriendHelpList_Ack_FriendList;

// ===================================================================

class FriendHelpList_Ack_FriendList : public ::google::protobuf::Message {
 public:
  FriendHelpList_Ack_FriendList();
  virtual ~FriendHelpList_Ack_FriendList();

  FriendHelpList_Ack_FriendList(const FriendHelpList_Ack_FriendList& from);

  inline FriendHelpList_Ack_FriendList& operator=(const FriendHelpList_Ack_FriendList& from) {
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
  static const FriendHelpList_Ack_FriendList& default_instance();

  void Swap(FriendHelpList_Ack_FriendList* other);

  // implements Message ----------------------------------------------

  FriendHelpList_Ack_FriendList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FriendHelpList_Ack_FriendList& from);
  void MergeFrom(const FriendHelpList_Ack_FriendList& from);
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

  // optional string nickname = 1;
  inline bool has_nickname() const;
  inline void clear_nickname();
  static const int kNicknameFieldNumber = 1;
  inline const ::std::string& nickname() const;
  inline void set_nickname(const ::std::string& value);
  inline void set_nickname(const char* value);
  inline void set_nickname(const char* value, size_t size);
  inline ::std::string* mutable_nickname();
  inline ::std::string* release_nickname();
  inline void set_allocated_nickname(::std::string* nickname);

  // optional int32 roleType = 2;
  inline bool has_roletype() const;
  inline void clear_roletype();
  static const int kRoleTypeFieldNumber = 2;
  inline ::google::protobuf::int32 roletype() const;
  inline void set_roletype(::google::protobuf::int32 value);

  // optional int32 roleLevel = 3;
  inline bool has_rolelevel() const;
  inline void clear_rolelevel();
  static const int kRoleLevelFieldNumber = 3;
  inline ::google::protobuf::int32 rolelevel() const;
  inline void set_rolelevel(::google::protobuf::int32 value);

  // optional int32 roleHeart = 4;
  inline bool has_roleheart() const;
  inline void clear_roleheart();
  static const int kRoleHeartFieldNumber = 4;
  inline ::google::protobuf::int32 roleheart() const;
  inline void set_roleheart(::google::protobuf::int32 value);

  // optional int32 roleHelp = 5;
  inline bool has_rolehelp() const;
  inline void clear_rolehelp();
  static const int kRoleHelpFieldNumber = 5;
  inline ::google::protobuf::int32 rolehelp() const;
  inline void set_rolehelp(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf.FriendHelpList_Ack.FriendList)
 private:
  inline void set_has_nickname();
  inline void clear_has_nickname();
  inline void set_has_roletype();
  inline void clear_has_roletype();
  inline void set_has_rolelevel();
  inline void clear_has_rolelevel();
  inline void set_has_roleheart();
  inline void clear_has_roleheart();
  inline void set_has_rolehelp();
  inline void clear_has_rolehelp();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* nickname_;
  ::google::protobuf::int32 roletype_;
  ::google::protobuf::int32 rolelevel_;
  ::google::protobuf::int32 roleheart_;
  ::google::protobuf::int32 rolehelp_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_FriendHelpList_5fAck_2eproto();
  friend void protobuf_AssignDesc_FriendHelpList_5fAck_2eproto();
  friend void protobuf_ShutdownFile_FriendHelpList_5fAck_2eproto();

  void InitAsDefaultInstance();
  static FriendHelpList_Ack_FriendList* default_instance_;
};
// -------------------------------------------------------------------

class FriendHelpList_Ack : public ::google::protobuf::Message {
 public:
  FriendHelpList_Ack();
  virtual ~FriendHelpList_Ack();

  FriendHelpList_Ack(const FriendHelpList_Ack& from);

  inline FriendHelpList_Ack& operator=(const FriendHelpList_Ack& from) {
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
  static const FriendHelpList_Ack& default_instance();

  void Swap(FriendHelpList_Ack* other);

  // implements Message ----------------------------------------------

  FriendHelpList_Ack* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FriendHelpList_Ack& from);
  void MergeFrom(const FriendHelpList_Ack& from);
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

  typedef FriendHelpList_Ack_FriendList FriendList;

  // accessors -------------------------------------------------------

  // optional string msgInfo = 1;
  inline bool has_msginfo() const;
  inline void clear_msginfo();
  static const int kMsgInfoFieldNumber = 1;
  inline const ::std::string& msginfo() const;
  inline void set_msginfo(const ::std::string& value);
  inline void set_msginfo(const char* value);
  inline void set_msginfo(const char* value, size_t size);
  inline ::std::string* mutable_msginfo();
  inline ::std::string* release_msginfo();
  inline void set_allocated_msginfo(::std::string* msginfo);

  // repeated .protobuf.FriendHelpList_Ack.FriendList friendList = 2;
  inline int friendlist_size() const;
  inline void clear_friendlist();
  static const int kFriendListFieldNumber = 2;
  inline const ::protobuf::FriendHelpList_Ack_FriendList& friendlist(int index) const;
  inline ::protobuf::FriendHelpList_Ack_FriendList* mutable_friendlist(int index);
  inline ::protobuf::FriendHelpList_Ack_FriendList* add_friendlist();
  inline const ::google::protobuf::RepeatedPtrField< ::protobuf::FriendHelpList_Ack_FriendList >&
      friendlist() const;
  inline ::google::protobuf::RepeatedPtrField< ::protobuf::FriendHelpList_Ack_FriendList >*
      mutable_friendlist();

  // optional int32 acktime = 3;
  inline bool has_acktime() const;
  inline void clear_acktime();
  static const int kAcktimeFieldNumber = 3;
  inline ::google::protobuf::int32 acktime() const;
  inline void set_acktime(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf.FriendHelpList_Ack)
 private:
  inline void set_has_msginfo();
  inline void clear_has_msginfo();
  inline void set_has_acktime();
  inline void clear_has_acktime();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* msginfo_;
  ::google::protobuf::RepeatedPtrField< ::protobuf::FriendHelpList_Ack_FriendList > friendlist_;
  ::google::protobuf::int32 acktime_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_FriendHelpList_5fAck_2eproto();
  friend void protobuf_AssignDesc_FriendHelpList_5fAck_2eproto();
  friend void protobuf_ShutdownFile_FriendHelpList_5fAck_2eproto();

  void InitAsDefaultInstance();
  static FriendHelpList_Ack* default_instance_;
};
// ===================================================================


// ===================================================================

// FriendHelpList_Ack_FriendList

// optional string nickname = 1;
inline bool FriendHelpList_Ack_FriendList::has_nickname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void FriendHelpList_Ack_FriendList::set_has_nickname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void FriendHelpList_Ack_FriendList::clear_has_nickname() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void FriendHelpList_Ack_FriendList::clear_nickname() {
  if (nickname_ != &::google::protobuf::internal::kEmptyString) {
    nickname_->clear();
  }
  clear_has_nickname();
}
inline const ::std::string& FriendHelpList_Ack_FriendList::nickname() const {
  return *nickname_;
}
inline void FriendHelpList_Ack_FriendList::set_nickname(const ::std::string& value) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(value);
}
inline void FriendHelpList_Ack_FriendList::set_nickname(const char* value) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(value);
}
inline void FriendHelpList_Ack_FriendList::set_nickname(const char* value, size_t size) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FriendHelpList_Ack_FriendList::mutable_nickname() {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  return nickname_;
}
inline ::std::string* FriendHelpList_Ack_FriendList::release_nickname() {
  clear_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = nickname_;
    nickname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void FriendHelpList_Ack_FriendList::set_allocated_nickname(::std::string* nickname) {
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

// optional int32 roleType = 2;
inline bool FriendHelpList_Ack_FriendList::has_roletype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void FriendHelpList_Ack_FriendList::set_has_roletype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void FriendHelpList_Ack_FriendList::clear_has_roletype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void FriendHelpList_Ack_FriendList::clear_roletype() {
  roletype_ = 0;
  clear_has_roletype();
}
inline ::google::protobuf::int32 FriendHelpList_Ack_FriendList::roletype() const {
  return roletype_;
}
inline void FriendHelpList_Ack_FriendList::set_roletype(::google::protobuf::int32 value) {
  set_has_roletype();
  roletype_ = value;
}

// optional int32 roleLevel = 3;
inline bool FriendHelpList_Ack_FriendList::has_rolelevel() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void FriendHelpList_Ack_FriendList::set_has_rolelevel() {
  _has_bits_[0] |= 0x00000004u;
}
inline void FriendHelpList_Ack_FriendList::clear_has_rolelevel() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void FriendHelpList_Ack_FriendList::clear_rolelevel() {
  rolelevel_ = 0;
  clear_has_rolelevel();
}
inline ::google::protobuf::int32 FriendHelpList_Ack_FriendList::rolelevel() const {
  return rolelevel_;
}
inline void FriendHelpList_Ack_FriendList::set_rolelevel(::google::protobuf::int32 value) {
  set_has_rolelevel();
  rolelevel_ = value;
}

// optional int32 roleHeart = 4;
inline bool FriendHelpList_Ack_FriendList::has_roleheart() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void FriendHelpList_Ack_FriendList::set_has_roleheart() {
  _has_bits_[0] |= 0x00000008u;
}
inline void FriendHelpList_Ack_FriendList::clear_has_roleheart() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void FriendHelpList_Ack_FriendList::clear_roleheart() {
  roleheart_ = 0;
  clear_has_roleheart();
}
inline ::google::protobuf::int32 FriendHelpList_Ack_FriendList::roleheart() const {
  return roleheart_;
}
inline void FriendHelpList_Ack_FriendList::set_roleheart(::google::protobuf::int32 value) {
  set_has_roleheart();
  roleheart_ = value;
}

// optional int32 roleHelp = 5;
inline bool FriendHelpList_Ack_FriendList::has_rolehelp() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void FriendHelpList_Ack_FriendList::set_has_rolehelp() {
  _has_bits_[0] |= 0x00000010u;
}
inline void FriendHelpList_Ack_FriendList::clear_has_rolehelp() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void FriendHelpList_Ack_FriendList::clear_rolehelp() {
  rolehelp_ = 0;
  clear_has_rolehelp();
}
inline ::google::protobuf::int32 FriendHelpList_Ack_FriendList::rolehelp() const {
  return rolehelp_;
}
inline void FriendHelpList_Ack_FriendList::set_rolehelp(::google::protobuf::int32 value) {
  set_has_rolehelp();
  rolehelp_ = value;
}

// -------------------------------------------------------------------

// FriendHelpList_Ack

// optional string msgInfo = 1;
inline bool FriendHelpList_Ack::has_msginfo() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void FriendHelpList_Ack::set_has_msginfo() {
  _has_bits_[0] |= 0x00000001u;
}
inline void FriendHelpList_Ack::clear_has_msginfo() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void FriendHelpList_Ack::clear_msginfo() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    msginfo_->clear();
  }
  clear_has_msginfo();
}
inline const ::std::string& FriendHelpList_Ack::msginfo() const {
  return *msginfo_;
}
inline void FriendHelpList_Ack::set_msginfo(const ::std::string& value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void FriendHelpList_Ack::set_msginfo(const char* value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void FriendHelpList_Ack::set_msginfo(const char* value, size_t size) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FriendHelpList_Ack::mutable_msginfo() {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  return msginfo_;
}
inline ::std::string* FriendHelpList_Ack::release_msginfo() {
  clear_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = msginfo_;
    msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void FriendHelpList_Ack::set_allocated_msginfo(::std::string* msginfo) {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    delete msginfo_;
  }
  if (msginfo) {
    set_has_msginfo();
    msginfo_ = msginfo;
  } else {
    clear_has_msginfo();
    msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// repeated .protobuf.FriendHelpList_Ack.FriendList friendList = 2;
inline int FriendHelpList_Ack::friendlist_size() const {
  return friendlist_.size();
}
inline void FriendHelpList_Ack::clear_friendlist() {
  friendlist_.Clear();
}
inline const ::protobuf::FriendHelpList_Ack_FriendList& FriendHelpList_Ack::friendlist(int index) const {
  return friendlist_.Get(index);
}
inline ::protobuf::FriendHelpList_Ack_FriendList* FriendHelpList_Ack::mutable_friendlist(int index) {
  return friendlist_.Mutable(index);
}
inline ::protobuf::FriendHelpList_Ack_FriendList* FriendHelpList_Ack::add_friendlist() {
  return friendlist_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::protobuf::FriendHelpList_Ack_FriendList >&
FriendHelpList_Ack::friendlist() const {
  return friendlist_;
}
inline ::google::protobuf::RepeatedPtrField< ::protobuf::FriendHelpList_Ack_FriendList >*
FriendHelpList_Ack::mutable_friendlist() {
  return &friendlist_;
}

// optional int32 acktime = 3;
inline bool FriendHelpList_Ack::has_acktime() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void FriendHelpList_Ack::set_has_acktime() {
  _has_bits_[0] |= 0x00000004u;
}
inline void FriendHelpList_Ack::clear_has_acktime() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void FriendHelpList_Ack::clear_acktime() {
  acktime_ = 0;
  clear_has_acktime();
}
inline ::google::protobuf::int32 FriendHelpList_Ack::acktime() const {
  return acktime_;
}
inline void FriendHelpList_Ack::set_acktime(::google::protobuf::int32 value) {
  set_has_acktime();
  acktime_ = value;
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

#endif  // PROTOBUF_FriendHelpList_5fAck_2eproto__INCLUDED
