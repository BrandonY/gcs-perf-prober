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

#include "perftest_config.hpp"

#include "absl/flags/flag.h"
#include "absl/strings/str_cat.h"

#include <iostream>
#include <optional>

ABSL_FLAG(std::optional<std::string>, universe, std::nullopt, "prod or preprod");
ABSL_FLAG(std::optional<std::string>, scenario, std::nullopt, "Name of the scenario, for example 100k-object-read");
ABSL_FLAG(std::optional<std::string>, region, std::nullopt, "Name of the scenario, for example 100k-object-read");
ABSL_FLAG(std::optional<std::string>, api, std::nullopt, "grpc, grpc-no-directparth, json");

ABSL_FLAG(std::optional<std::string>, operation, std::nullopt, "ReadObject, WriteObject, etc");
ABSL_FLAG(std::optional<std::string>, object_name, std::nullopt, "Name of object to read/write");

ABSL_FLAG(std::optional<long>, write_length, std::nullopt, "For write operations, object size");

std::optional<PerftestConfig> PerftestConfig::LoadConfig()
{
    if (!absl::GetFlag(FLAGS_universe).has_value())
    {
        std::cerr << "You must set the 'universe' flag to 'prod' or 'preprod'" << std::endl;
        return std::nullopt;
    }
    if (!absl::GetFlag(FLAGS_api).has_value())
    {
        std::cerr << "You must set the 'api' flag to 'grpc', 'grpc-no-directpath', or 'json'" << std::endl;
        return std::nullopt;
    }
    if (!absl::GetFlag(FLAGS_scenario).has_value())
    {
        std::cerr << "Must specify 'scenario'" << std::endl;
        return std::nullopt;
    }
    if (!absl::GetFlag(FLAGS_region).has_value())
    {
        std::cerr << "Must specify 'region'" << std::endl;
        return std::nullopt;
    }
    if (!absl::GetFlag(FLAGS_operation).has_value())
    {
        std::cerr << "Must specify 'operation'" << std::endl;
        return std::nullopt;
    }
    if (!absl::GetFlag(FLAGS_object_name).has_value())
    {
        std::cerr << "Must specify 'object_name'" << std::endl;
        return std::nullopt;
    }

    ClientAPI api;
    if (absl::GetFlag(FLAGS_api) == "grpc")
    {
        api = GRPC_DIRECTPATH;
    }
    else if (absl::GetFlag(FLAGS_api) == "json")
    {
        api = JSON;
    }
    else
    {
        std::cerr << "You must set the 'api' flag to 'grpc', 'grpc-no-directpath', or 'json'" << std::endl;
        return std::nullopt;
    }

    std::string bucket_prefix;

    Universe universe;
    if (absl::GetFlag(FLAGS_universe) == "prod")
    {
        universe = PROD;
        bucket_prefix = "gcs-grpc-team-perf-testing";
    }
    else if (absl::GetFlag(FLAGS_universe) == "preprod")
    {
        universe = PREPROD;
        bucket_prefix = "gcs-grpc-team-preprod-perf";
    }
    else
    {
        std::cerr << "You must set the 'universe' flag to 'prod' or 'preprod'" << std::endl;
        return std::nullopt;
    }

    Operation operation;
    if (absl::GetFlag(FLAGS_operation) == "ReadObject")
    {
        operation = READ;
    }
    else if (absl::GetFlag(FLAGS_operation) == "OneShotWriteObject")
    {
        operation = ONESHOT_WRITE;
    }
    else if (absl::GetFlag(FLAGS_operation) == "ResumableWriteObject")
    {
        operation = RESUMABLE_WRITE;
    }
    else
    {
        std::cerr << "You must set the 'operation' flag to a known operation" << std::endl;
        return std::nullopt;
    }

    long write_length = 0;
    if (operation == ONESHOT_WRITE || operation == RESUMABLE_WRITE)
    {
        if (!absl::GetFlag(FLAGS_write_length).has_value())
        {
            std::cerr << "You must set the a 'write_length' in bytes for write calls" << std::endl;
            return std::nullopt;
        }
        write_length = absl::GetFlag(FLAGS_write_length).value();
    }

    std::string scenario = *absl::GetFlag(FLAGS_scenario);
    std::string region = *absl::GetFlag(FLAGS_region);
    std::string bucket = absl::StrCat(bucket_prefix, "-", region, "-", scenario);
    std::string object = *absl::GetFlag(FLAGS_object_name);

    return std::make_optional<PerftestConfig>({scenario, region,
                                               universe, api, operation, bucket, object, write_length});
}

std::string PerftestConfig::universe_str() {
    switch(universe_) {
        case PROD: return "prod";
        case PREPROD: return "preprod";
        case HP_PREPROD: return "hp-preprod";
    }
    return nullptr;
}

std::string PerftestConfig::clientAPI_str() {
    switch(clientAPI_) {
        case GRPC_DIRECTPATH: return "gRPC";
        case JSON: return "JSON";
    }
    return nullptr;
}
