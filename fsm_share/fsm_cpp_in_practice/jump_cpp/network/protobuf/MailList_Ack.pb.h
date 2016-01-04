// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MailList_Ack.proto

#ifndef PROTOBUF_MailList_5fAck_2eproto__INCLUDED
#define PROTOBUF_MailList_5fAck_2eproto__INCLUDED

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
void  protobuf_AddDesc_MailList_5fAck_2eproto();
void protobuf_AssignDesc_MailList_5fAck_2eproto();
void protobuf_ShutdownFile_MailList_5fAck_2eproto();

class MailList_Ack;
class MailList_Ack_MailList;

// ===================================================================

class MailList_Ack_MailList : public ::google::protobuf::Message {
 public:
  MailList_Ack_MailList();
  virtual ~MailList_Ack_MailList();

  MailList_Ack_MailList(const MailList_Ack_MailList& from);

  inline MailList_Ack_MailList& operator=(const MailList_Ack_MailList& from) {
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
  static const MailList_Ack_MailList& default_instance();

  void Swap(MailList_Ack_MailList* other);

  // implements Message ----------------------------------------------

  MailList_Ack_MailList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MailList_Ack_MailList& from);
  void MergeFrom(const MailList_Ack_MailList& from);
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

  // optional int32 mailId = 1;
  inline bool has_mailid() const;
  inline void clear_mailid();
  static const int kMailIdFieldNumber = 1;
  inline ::google::protobuf::int32 mailid() const;
  inline void set_mailid(::google::protobuf::int32 value);

  // optional int32 mailType = 2;
  inline bool has_mailtype() const;
  inline void clear_mailtype();
  static const int kMailTypeFieldNumber = 2;
  inline ::google::protobuf::int32 mailtype() const;
  inline void set_mailtype(::google::protobuf::int32 value);

  // optional int32 playerId = 3;
  inline bool has_playerid() const;
  inline void clear_playerid();
  static const int kPlayerIdFieldNumber = 3;
  inline ::google::protobuf::int32 playerid() const;
  inline void set_playerid(::google::protobuf::int32 value);

  // optional string nickname = 4;
  inline bool has_nickname() const;
  inline void clear_nickname();
  static const int kNicknameFieldNumber = 4;
  inline const ::std::string& nickname() const;
  inline void set_nickname(const ::std::string& value);
  inline void set_nickname(const char* value);
  inline void set_nickname(const char* value, size_t size);
  inline ::std::string* mutable_nickname();
  inline ::std::string* release_nickname();
  inline void set_allocated_nickname(::std::string* nickname);

  // optional int32 itemId = 5;
  inline bool has_itemid() const;
  inline void clear_itemid();
  static const int kItemIdFieldNumber = 5;
  inline ::google::protobuf::int32 itemid() const;
  inline void set_itemid(::google::protobuf::int32 value);

  // optional int32 itemNum = 6;
  inline bool has_itemnum() const;
  inline void clear_itemnum();
  static const int kItemNumFieldNumber = 6;
  inline ::google::protobuf::int32 itemnum() const;
  inline void set_itemnum(::google::protobuf::int32 value);

  // optional string title = 7;
  inline bool has_title() const;
  inline void clear_title();
  static const int kTitleFieldNumber = 7;
  inline const ::std::string& title() const;
  inline void set_title(const ::std::string& value);
  inline void set_title(const char* value);
  inline void set_title(const char* value, size_t size);
  inline ::std::string* mutable_title();
  inline ::std::string* release_title();
  inline void set_allocated_title(::std::string* title);

  // optional string contents = 8;
  inline bool has_contents() const;
  inline void clear_contents();
  static const int kContentsFieldNumber = 8;
  inline const ::std::string& contents() const;
  inline void set_contents(const ::std::string& value);
  inline void set_contents(const char* value);
  inline void set_contents(const char* value, size_t size);
  inline ::std::string* mutable_contents();
  inline ::std::string* release_contents();
  inline void set_allocated_contents(::std::string* contents);

  // optional int32 status = 9;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 9;
  inline ::google::protobuf::int32 status() const;
  inline void set_status(::google::protobuf::int32 value);

  // optional string domainTime = 10;
  inline bool has_domaintime() const;
  inline void clear_domaintime();
  static const int kDomainTimeFieldNumber = 10;
  inline const ::std::string& domaintime() const;
  inline void set_domaintime(const ::std::string& value);
  inline void set_domaintime(const char* value);
  inline void set_domaintime(const char* value, size_t size);
  inline ::std::string* mutable_domaintime();
  inline ::std::string* release_domaintime();
  inline void set_allocated_domaintime(::std::string* domaintime);

  // @@protoc_insertion_point(class_scope:protobuf.MailList_Ack.MailList)
 private:
  inline void set_has_mailid();
  inline void clear_has_mailid();
  inline void set_has_mailtype();
  inline void clear_has_mailtype();
  inline void set_has_playerid();
  inline void clear_has_playerid();
  inline void set_has_nickname();
  inline void clear_has_nickname();
  inline void set_has_itemid();
  inline void clear_has_itemid();
  inline void set_has_itemnum();
  inline void clear_has_itemnum();
  inline void set_has_title();
  inline void clear_has_title();
  inline void set_has_contents();
  inline void clear_has_contents();
  inline void set_has_status();
  inline void clear_has_status();
  inline void set_has_domaintime();
  inline void clear_has_domaintime();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 mailid_;
  ::google::protobuf::int32 mailtype_;
  ::std::string* nickname_;
  ::google::protobuf::int32 playerid_;
  ::google::protobuf::int32 itemid_;
  ::std::string* title_;
  ::google::protobuf::int32 itemnum_;
  ::google::protobuf::int32 status_;
  ::std::string* contents_;
  ::std::string* domaintime_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(10 + 31) / 32];

  friend void  protobuf_AddDesc_MailList_5fAck_2eproto();
  friend void protobuf_AssignDesc_MailList_5fAck_2eproto();
  friend void protobuf_ShutdownFile_MailList_5fAck_2eproto();

  void InitAsDefaultInstance();
  static MailList_Ack_MailList* default_instance_;
};
// -------------------------------------------------------------------

class MailList_Ack : public ::google::protobuf::Message {
 public:
  MailList_Ack();
  virtual ~MailList_Ack();

  MailList_Ack(const MailList_Ack& from);

  inline MailList_Ack& operator=(const MailList_Ack& from) {
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
  static const MailList_Ack& default_instance();

  void Swap(MailList_Ack* other);

  // implements Message ----------------------------------------------

  MailList_Ack* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MailList_Ack& from);
  void MergeFrom(const MailList_Ack& from);
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

  typedef MailList_Ack_MailList MailList;

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

  // required int32 mailCount = 2;
  inline bool has_mailcount() const;
  inline void clear_mailcount();
  static const int kMailCountFieldNumber = 2;
  inline ::google::protobuf::int32 mailcount() const;
  inline void set_mailcount(::google::protobuf::int32 value);

  // repeated .protobuf.MailList_Ack.MailList mailList = 3;
  inline int maillist_size() const;
  inline void clear_maillist();
  static const int kMailListFieldNumber = 3;
  inline const ::protobuf::MailList_Ack_MailList& maillist(int index) const;
  inline ::protobuf::MailList_Ack_MailList* mutable_maillist(int index);
  inline ::protobuf::MailList_Ack_MailList* add_maillist();
  inline const ::google::protobuf::RepeatedPtrField< ::protobuf::MailList_Ack_MailList >&
      maillist() const;
  inline ::google::protobuf::RepeatedPtrField< ::protobuf::MailList_Ack_MailList >*
      mutable_maillist();

  // required int32 acktime = 4;
  inline bool has_acktime() const;
  inline void clear_acktime();
  static const int kAcktimeFieldNumber = 4;
  inline ::google::protobuf::int32 acktime() const;
  inline void set_acktime(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf.MailList_Ack)
 private:
  inline void set_has_msginfo();
  inline void clear_has_msginfo();
  inline void set_has_mailcount();
  inline void clear_has_mailcount();
  inline void set_has_acktime();
  inline void clear_has_acktime();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* msginfo_;
  ::google::protobuf::RepeatedPtrField< ::protobuf::MailList_Ack_MailList > maillist_;
  ::google::protobuf::int32 mailcount_;
  ::google::protobuf::int32 acktime_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_MailList_5fAck_2eproto();
  friend void protobuf_AssignDesc_MailList_5fAck_2eproto();
  friend void protobuf_ShutdownFile_MailList_5fAck_2eproto();

  void InitAsDefaultInstance();
  static MailList_Ack* default_instance_;
};
// ===================================================================


// ===================================================================

// MailList_Ack_MailList

// optional int32 mailId = 1;
inline bool MailList_Ack_MailList::has_mailid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void MailList_Ack_MailList::set_has_mailid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void MailList_Ack_MailList::clear_has_mailid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void MailList_Ack_MailList::clear_mailid() {
  mailid_ = 0;
  clear_has_mailid();
}
inline ::google::protobuf::int32 MailList_Ack_MailList::mailid() const {
  return mailid_;
}
inline void MailList_Ack_MailList::set_mailid(::google::protobuf::int32 value) {
  set_has_mailid();
  mailid_ = value;
}

// optional int32 mailType = 2;
inline bool MailList_Ack_MailList::has_mailtype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void MailList_Ack_MailList::set_has_mailtype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void MailList_Ack_MailList::clear_has_mailtype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void MailList_Ack_MailList::clear_mailtype() {
  mailtype_ = 0;
  clear_has_mailtype();
}
inline ::google::protobuf::int32 MailList_Ack_MailList::mailtype() const {
  return mailtype_;
}
inline void MailList_Ack_MailList::set_mailtype(::google::protobuf::int32 value) {
  set_has_mailtype();
  mailtype_ = value;
}

// optional int32 playerId = 3;
inline bool MailList_Ack_MailList::has_playerid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void MailList_Ack_MailList::set_has_playerid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void MailList_Ack_MailList::clear_has_playerid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void MailList_Ack_MailList::clear_playerid() {
  playerid_ = 0;
  clear_has_playerid();
}
inline ::google::protobuf::int32 MailList_Ack_MailList::playerid() const {
  return playerid_;
}
inline void MailList_Ack_MailList::set_playerid(::google::protobuf::int32 value) {
  set_has_playerid();
  playerid_ = value;
}

// optional string nickname = 4;
inline bool MailList_Ack_MailList::has_nickname() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void MailList_Ack_MailList::set_has_nickname() {
  _has_bits_[0] |= 0x00000008u;
}
inline void MailList_Ack_MailList::clear_has_nickname() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void MailList_Ack_MailList::clear_nickname() {
  if (nickname_ != &::google::protobuf::internal::kEmptyString) {
    nickname_->clear();
  }
  clear_has_nickname();
}
inline const ::std::string& MailList_Ack_MailList::nickname() const {
  return *nickname_;
}
inline void MailList_Ack_MailList::set_nickname(const ::std::string& value) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(value);
}
inline void MailList_Ack_MailList::set_nickname(const char* value) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(value);
}
inline void MailList_Ack_MailList::set_nickname(const char* value, size_t size) {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  nickname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MailList_Ack_MailList::mutable_nickname() {
  set_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    nickname_ = new ::std::string;
  }
  return nickname_;
}
inline ::std::string* MailList_Ack_MailList::release_nickname() {
  clear_has_nickname();
  if (nickname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = nickname_;
    nickname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void MailList_Ack_MailList::set_allocated_nickname(::std::string* nickname) {
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

// optional int32 itemId = 5;
inline bool MailList_Ack_MailList::has_itemid() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void MailList_Ack_MailList::set_has_itemid() {
  _has_bits_[0] |= 0x00000010u;
}
inline void MailList_Ack_MailList::clear_has_itemid() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void MailList_Ack_MailList::clear_itemid() {
  itemid_ = 0;
  clear_has_itemid();
}
inline ::google::protobuf::int32 MailList_Ack_MailList::itemid() const {
  return itemid_;
}
inline void MailList_Ack_MailList::set_itemid(::google::protobuf::int32 value) {
  set_has_itemid();
  itemid_ = value;
}

// optional int32 itemNum = 6;
inline bool MailList_Ack_MailList::has_itemnum() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void MailList_Ack_MailList::set_has_itemnum() {
  _has_bits_[0] |= 0x00000020u;
}
inline void MailList_Ack_MailList::clear_has_itemnum() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void MailList_Ack_MailList::clear_itemnum() {
  itemnum_ = 0;
  clear_has_itemnum();
}
inline ::google::protobuf::int32 MailList_Ack_MailList::itemnum() const {
  return itemnum_;
}
inline void MailList_Ack_MailList::set_itemnum(::google::protobuf::int32 value) {
  set_has_itemnum();
  itemnum_ = value;
}

// optional string title = 7;
inline bool MailList_Ack_MailList::has_title() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void MailList_Ack_MailList::set_has_title() {
  _has_bits_[0] |= 0x00000040u;
}
inline void MailList_Ack_MailList::clear_has_title() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void MailList_Ack_MailList::clear_title() {
  if (title_ != &::google::protobuf::internal::kEmptyString) {
    title_->clear();
  }
  clear_has_title();
}
inline const ::std::string& MailList_Ack_MailList::title() const {
  return *title_;
}
inline void MailList_Ack_MailList::set_title(const ::std::string& value) {
  set_has_title();
  if (title_ == &::google::protobuf::internal::kEmptyString) {
    title_ = new ::std::string;
  }
  title_->assign(value);
}
inline void MailList_Ack_MailList::set_title(const char* value) {
  set_has_title();
  if (title_ == &::google::protobuf::internal::kEmptyString) {
    title_ = new ::std::string;
  }
  title_->assign(value);
}
inline void MailList_Ack_MailList::set_title(const char* value, size_t size) {
  set_has_title();
  if (title_ == &::google::protobuf::internal::kEmptyString) {
    title_ = new ::std::string;
  }
  title_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MailList_Ack_MailList::mutable_title() {
  set_has_title();
  if (title_ == &::google::protobuf::internal::kEmptyString) {
    title_ = new ::std::string;
  }
  return title_;
}
inline ::std::string* MailList_Ack_MailList::release_title() {
  clear_has_title();
  if (title_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = title_;
    title_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void MailList_Ack_MailList::set_allocated_title(::std::string* title) {
  if (title_ != &::google::protobuf::internal::kEmptyString) {
    delete title_;
  }
  if (title) {
    set_has_title();
    title_ = title;
  } else {
    clear_has_title();
    title_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string contents = 8;
inline bool MailList_Ack_MailList::has_contents() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void MailList_Ack_MailList::set_has_contents() {
  _has_bits_[0] |= 0x00000080u;
}
inline void MailList_Ack_MailList::clear_has_contents() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void MailList_Ack_MailList::clear_contents() {
  if (contents_ != &::google::protobuf::internal::kEmptyString) {
    contents_->clear();
  }
  clear_has_contents();
}
inline const ::std::string& MailList_Ack_MailList::contents() const {
  return *contents_;
}
inline void MailList_Ack_MailList::set_contents(const ::std::string& value) {
  set_has_contents();
  if (contents_ == &::google::protobuf::internal::kEmptyString) {
    contents_ = new ::std::string;
  }
  contents_->assign(value);
}
inline void MailList_Ack_MailList::set_contents(const char* value) {
  set_has_contents();
  if (contents_ == &::google::protobuf::internal::kEmptyString) {
    contents_ = new ::std::string;
  }
  contents_->assign(value);
}
inline void MailList_Ack_MailList::set_contents(const char* value, size_t size) {
  set_has_contents();
  if (contents_ == &::google::protobuf::internal::kEmptyString) {
    contents_ = new ::std::string;
  }
  contents_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MailList_Ack_MailList::mutable_contents() {
  set_has_contents();
  if (contents_ == &::google::protobuf::internal::kEmptyString) {
    contents_ = new ::std::string;
  }
  return contents_;
}
inline ::std::string* MailList_Ack_MailList::release_contents() {
  clear_has_contents();
  if (contents_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = contents_;
    contents_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void MailList_Ack_MailList::set_allocated_contents(::std::string* contents) {
  if (contents_ != &::google::protobuf::internal::kEmptyString) {
    delete contents_;
  }
  if (contents) {
    set_has_contents();
    contents_ = contents;
  } else {
    clear_has_contents();
    contents_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 status = 9;
inline bool MailList_Ack_MailList::has_status() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void MailList_Ack_MailList::set_has_status() {
  _has_bits_[0] |= 0x00000100u;
}
inline void MailList_Ack_MailList::clear_has_status() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void MailList_Ack_MailList::clear_status() {
  status_ = 0;
  clear_has_status();
}
inline ::google::protobuf::int32 MailList_Ack_MailList::status() const {
  return status_;
}
inline void MailList_Ack_MailList::set_status(::google::protobuf::int32 value) {
  set_has_status();
  status_ = value;
}

// optional string domainTime = 10;
inline bool MailList_Ack_MailList::has_domaintime() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void MailList_Ack_MailList::set_has_domaintime() {
  _has_bits_[0] |= 0x00000200u;
}
inline void MailList_Ack_MailList::clear_has_domaintime() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void MailList_Ack_MailList::clear_domaintime() {
  if (domaintime_ != &::google::protobuf::internal::kEmptyString) {
    domaintime_->clear();
  }
  clear_has_domaintime();
}
inline const ::std::string& MailList_Ack_MailList::domaintime() const {
  return *domaintime_;
}
inline void MailList_Ack_MailList::set_domaintime(const ::std::string& value) {
  set_has_domaintime();
  if (domaintime_ == &::google::protobuf::internal::kEmptyString) {
    domaintime_ = new ::std::string;
  }
  domaintime_->assign(value);
}
inline void MailList_Ack_MailList::set_domaintime(const char* value) {
  set_has_domaintime();
  if (domaintime_ == &::google::protobuf::internal::kEmptyString) {
    domaintime_ = new ::std::string;
  }
  domaintime_->assign(value);
}
inline void MailList_Ack_MailList::set_domaintime(const char* value, size_t size) {
  set_has_domaintime();
  if (domaintime_ == &::google::protobuf::internal::kEmptyString) {
    domaintime_ = new ::std::string;
  }
  domaintime_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MailList_Ack_MailList::mutable_domaintime() {
  set_has_domaintime();
  if (domaintime_ == &::google::protobuf::internal::kEmptyString) {
    domaintime_ = new ::std::string;
  }
  return domaintime_;
}
inline ::std::string* MailList_Ack_MailList::release_domaintime() {
  clear_has_domaintime();
  if (domaintime_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = domaintime_;
    domaintime_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void MailList_Ack_MailList::set_allocated_domaintime(::std::string* domaintime) {
  if (domaintime_ != &::google::protobuf::internal::kEmptyString) {
    delete domaintime_;
  }
  if (domaintime) {
    set_has_domaintime();
    domaintime_ = domaintime;
  } else {
    clear_has_domaintime();
    domaintime_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// MailList_Ack

// optional string msgInfo = 1;
inline bool MailList_Ack::has_msginfo() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void MailList_Ack::set_has_msginfo() {
  _has_bits_[0] |= 0x00000001u;
}
inline void MailList_Ack::clear_has_msginfo() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void MailList_Ack::clear_msginfo() {
  if (msginfo_ != &::google::protobuf::internal::kEmptyString) {
    msginfo_->clear();
  }
  clear_has_msginfo();
}
inline const ::std::string& MailList_Ack::msginfo() const {
  return *msginfo_;
}
inline void MailList_Ack::set_msginfo(const ::std::string& value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void MailList_Ack::set_msginfo(const char* value) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(value);
}
inline void MailList_Ack::set_msginfo(const char* value, size_t size) {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  msginfo_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MailList_Ack::mutable_msginfo() {
  set_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    msginfo_ = new ::std::string;
  }
  return msginfo_;
}
inline ::std::string* MailList_Ack::release_msginfo() {
  clear_has_msginfo();
  if (msginfo_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = msginfo_;
    msginfo_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void MailList_Ack::set_allocated_msginfo(::std::string* msginfo) {
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

// required int32 mailCount = 2;
inline bool MailList_Ack::has_mailcount() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void MailList_Ack::set_has_mailcount() {
  _has_bits_[0] |= 0x00000002u;
}
inline void MailList_Ack::clear_has_mailcount() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void MailList_Ack::clear_mailcount() {
  mailcount_ = 0;
  clear_has_mailcount();
}
inline ::google::protobuf::int32 MailList_Ack::mailcount() const {
  return mailcount_;
}
inline void MailList_Ack::set_mailcount(::google::protobuf::int32 value) {
  set_has_mailcount();
  mailcount_ = value;
}

// repeated .protobuf.MailList_Ack.MailList mailList = 3;
inline int MailList_Ack::maillist_size() const {
  return maillist_.size();
}
inline void MailList_Ack::clear_maillist() {
  maillist_.Clear();
}
inline const ::protobuf::MailList_Ack_MailList& MailList_Ack::maillist(int index) const {
  return maillist_.Get(index);
}
inline ::protobuf::MailList_Ack_MailList* MailList_Ack::mutable_maillist(int index) {
  return maillist_.Mutable(index);
}
inline ::protobuf::MailList_Ack_MailList* MailList_Ack::add_maillist() {
  return maillist_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::protobuf::MailList_Ack_MailList >&
MailList_Ack::maillist() const {
  return maillist_;
}
inline ::google::protobuf::RepeatedPtrField< ::protobuf::MailList_Ack_MailList >*
MailList_Ack::mutable_maillist() {
  return &maillist_;
}

// required int32 acktime = 4;
inline bool MailList_Ack::has_acktime() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void MailList_Ack::set_has_acktime() {
  _has_bits_[0] |= 0x00000008u;
}
inline void MailList_Ack::clear_has_acktime() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void MailList_Ack::clear_acktime() {
  acktime_ = 0;
  clear_has_acktime();
}
inline ::google::protobuf::int32 MailList_Ack::acktime() const {
  return acktime_;
}
inline void MailList_Ack::set_acktime(::google::protobuf::int32 value) {
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

#endif  // PROTOBUF_MailList_5fAck_2eproto__INCLUDED