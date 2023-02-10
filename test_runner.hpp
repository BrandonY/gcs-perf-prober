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

#ifndef __TEST_RUNNER_HPP__
#define __TEST_RUNNER_HPP__

#include "perftest_config.hpp"
#include "gcs_client.hpp"
#include "prometheus_reporter.hpp"

#include <memory>

class TestRunner
{
public:
    TestRunner(PerftestConfig config);

    void Run(PrometheusReporter *reporter);

private:
    PerftestConfig config_;
    std::unique_ptr<GcsClient> CreateClient();
};

#endif // __TEST_RUNNER_HPP__
