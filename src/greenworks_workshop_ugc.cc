// Copyright (c) 2019 Florian van Strien All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "greenworks_workshop_ugc.h"

#include <algorithm>

#include "nan.h"
#include "steam/steam_api.h"
#include "v8.h"

#include "greenworks_utils.h"

namespace greenworks {
    ItemCreatorWorker::ItemCreatorWorker(
    Nan::Callback* success_callback, Nan::Callback* error_callback,
    uint32 app_id)
    : SteamCallbackAsyncWorker(success_callback, error_callback),
      app_id_(app_id) {
          k_EWorkshopFileTypeCommunity
      }

    void ItemCreatorWorker::Execute() {
        SteamAPICall_t create_result = SteamUGC()->CreateItem(app_id_, k_EWorkshopFileTypeCommunity);

        call_result_.Set(create_result, this,
            &ItemCreatorWorker::OnCreateDone);
    }

    void ItemCreatorWorker::OnCreateDone(CreateItemResult_t result, bool io_failure) {
        if ( io_failure || !result->m_eResult == k_EResultOK)
        {
            SetErrorMessage("Failed to create workshop item.");
            return;
        }
        is_completed_ = true;
        publish_file_id_ = result->m_nPublishedFileId;
    }

    void ItemCreatorWorker::HandleOKCallback() {
        Nan::HandleScope scope;

        v8::Local<v8::Value> argv[] = {
            Nan::New(utils::uint64ToString(publish_file_id_)).ToLocalChecked() };
        callback->Call(1, argv);
    }
}