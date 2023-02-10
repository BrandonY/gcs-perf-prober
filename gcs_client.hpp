/* Copyright 2023 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __GCS_CLIENT_HPP_
#define __GCS_CLIENT_HPP_

#include <string>
#include <memory>

#include "google/cloud/storage/client.h"

#include "universes.hpp"

class GcsClient
{
public:
    static std::unique_ptr<GcsClient> MakeDirectpathClient(Universe universe, std::string bucket);
    static std::unique_ptr<GcsClient> MakeJSONClient(Universe universe, std::string bucket);

    // GcsClient(const GcsClient &) = delete;
    // GcsClient &operator=(const GcsClient &) = delete;

    bool ReadObject(std::string object);
    bool ResumablyWriteObject(std::string object, unsigned long bytes);
    bool OneShotWriteObject(std::string object, unsigned long bytes);
    void StartResumableWrite(std::string object);

    static std::string GRPCVersion();
    static std::string GCSClientVersion();

    // For logging/diagnostics
    std::string LastSessionId() { return last_session_id_; }

private:
    GcsClient(google::cloud::storage::Client client, std::string bucket);

    google::cloud::storage::Client client_;
    const std::string bucket_;
    unsigned long random_write_buffer_len_;
    std::unique_ptr<char[]> random_write_buffer_;

    std::vector<char> io_buffer_;

    std::string last_session_id_;
};

#endif // __GCS_CLIENT_HPP_