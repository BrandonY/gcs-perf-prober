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

#include "metrics_tracker.hpp"

#include <stmpct/ckms_tq.hpp>
#include <vector>
#include <iostream>
#include <algorithm>

MetricsTracker::MetricsTracker() : success_latency_tracker_(quantiles_.begin(), quantiles_.end())
{
}

void MetricsTracker::ReportSuccess(double latency)
{
    success_latency_tracker_.insert(latency);
    success_count_ += 1;
    request_count_ += 1;
}

void MetricsTracker::ReportError(double latency)
{
    request_count_ += 1;
}

double MetricsTracker::Quantile(double p)
{
    for (stmpct::targeted_quantile q : quantiles_)
    {
        if (q.phi == p)
        {
            return success_latency_tracker_.quantile(p);
            ;
        }
    }

    std::cerr << "Request for quantile " << p << ", which is not tracked.";
    exit(1);
    return 0;
}