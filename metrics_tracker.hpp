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

#ifndef metrics_tracker_hpp__
#define metrics_tracker_hpp__

#include <stmpct/ckms_tq.hpp>

class MetricsTracker
{
public:
  MetricsTracker();

  MetricsTracker(const MetricsTracker &) = delete;
  MetricsTracker &operator=(const MetricsTracker &) = delete;

  void ReportSuccess(double latency);
  void ReportError(double latency);

  double Quantile(double p);
  
  unsigned long successes() { return success_count_; }
  unsigned long requests() { return request_count_; }

private:
  unsigned long request_count_ = 0;
  unsigned long success_count_ = 0;
  std::vector<stmpct::targeted_quantile> quantiles_ = {{0.5, 0.0001}, {.9, .0001}, {.99, .0001}};
  stmpct::ckms_tq<double> success_latency_tracker_;
};

#endif // metrics_tracker_hpp__