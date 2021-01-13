//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2021
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/td_api.h"
#include "td/telegram/telegram_api.h"
#include "td/telegram/UserId.h"

#include "td/utils/common.h"
#include "td/utils/StringBuilder.h"
#include "td/utils/tl_helpers.h"

namespace td {

class ContactsManager;

class DialogInviteLink {
  string invite_link_;
  UserId administrator_user_id_;
  int32 date_ = 0;
  int32 expire_date_ = 0;
  int32 usage_limit_ = 0;
  int32 usage_count_ = 0;
  bool is_revoked_ = false;
  bool is_permanent_ = false;

  friend bool operator==(const DialogInviteLink &lhs, const DialogInviteLink &rhs);

  friend StringBuilder &operator<<(StringBuilder &string_builder, const DialogInviteLink &invite_link);

 public:
  DialogInviteLink() = default;

  DialogInviteLink(tl_object_ptr<telegram_api::chatInviteExported> exported_invite);

  td_api::object_ptr<td_api::chatInviteLink> get_chat_invite_link_object(const ContactsManager *contacts_manager) const;

  bool is_valid() const {
    return !invite_link_.empty() && administrator_user_id_.is_valid() && date_ > 0;
  }

  bool is_expired() const;

  int32 get_expire_time() const;

  UserId get_administrator_user_id() const {
    return administrator_user_id_;
  }

  template <class StorerT>
  void store(StorerT &storer) const {
    using td::store;
    bool has_expire_date = expire_date_ != 0;
    bool has_usage_limit = usage_limit_ != 0;
    bool has_usage_count = usage_count_ != 0;
    BEGIN_STORE_FLAGS();
    STORE_FLAG(is_revoked_);
    STORE_FLAG(is_permanent_);
    STORE_FLAG(has_expire_date);
    STORE_FLAG(has_usage_limit);
    STORE_FLAG(has_usage_count);
    END_STORE_FLAGS();
    store(invite_link_, storer);
    store(administrator_user_id_, storer);
    store(date_, storer);
    if (has_expire_date) {
      store(expire_date_, storer);
    }
    if (has_usage_limit) {
      store(usage_limit_, storer);
    }
    if (has_usage_count) {
      store(usage_count_, storer);
    }
  }

  template <class ParserT>
  void parse(ParserT &parser) {
    using td::parse;
    bool has_expire_date;
    bool has_usage_limit;
    bool has_usage_count;
    BEGIN_PARSE_FLAGS();
    PARSE_FLAG(is_revoked_);
    PARSE_FLAG(is_permanent_);
    PARSE_FLAG(has_expire_date);
    PARSE_FLAG(has_usage_limit);
    PARSE_FLAG(has_usage_count);
    END_PARSE_FLAGS();
    parse(invite_link_, parser);
    parse(administrator_user_id_, parser);
    parse(date_, parser);
    if (has_expire_date) {
      parse(expire_date_, parser);
    }
    if (has_usage_limit) {
      parse(usage_limit_, parser);
    }
    if (has_usage_count) {
      parse(usage_count_, parser);
    }
  }
};

bool operator==(const DialogInviteLink &lhs, const DialogInviteLink &rhs);

bool operator!=(const DialogInviteLink &lhs, const DialogInviteLink &rhs);

StringBuilder &operator<<(StringBuilder &string_builder, const DialogInviteLink &invite_link);

}  // namespace td
