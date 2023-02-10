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

#ifndef __PROMETHEUS_REPORTER_HPP_
#define __PROMETHEUS_REPORTER_HPP_

#include <string>
#include <memory>

#include <prometheus/gauge.h>
#include <prometheus/registry.h>
#include <prometheus/gateway.h>
#include <prometheus/counter.h>

class PrometheusReporter
{
public:
    PrometheusReporter(std::string host, std::string port,
                       std::string scenario, std::string api, std::string location, std::string universe, std::string grpc_version, std::string gcs_client_version);

    PrometheusReporter(const PrometheusReporter &) = delete;
    PrometheusReporter &operator=(const PrometheusReporter &) = delete;

    void RecordSuccessP50(double latency);
    void RecordSuccessP90(double latency);

    void ReportSuccesses(long count);
    void ReportCalls(long count);

    bool Push();
    void Summarize();

private:
    std::shared_ptr<prometheus::Registry> registry_;
    prometheus::Gateway gateway_;
    prometheus::Labels labels_;

    prometheus::Family<prometheus::Gauge> &success_p50_gauge_family_;
    prometheus::Gauge &success_p50_gauge_;

    prometheus::Family<prometheus::Gauge> &success_p90_gauge_family_;
    prometheus::Gauge &success_p90_gauge_;

    // These sure LOOK like counters, but perf-gauge treated them as gauges, so sure, let's do that.
    prometheus::Family<prometheus::Gauge> &success_count_family_;
    prometheus::Gauge &success_count_;

    prometheus::Family<prometheus::Gauge> &request_count_family_;
    prometheus::Gauge &request_count_;
};

#endif // __PROMETHEUS_REPORTER_HPP_