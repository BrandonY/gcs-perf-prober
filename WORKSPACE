#  Copyright 2023 Google LLC
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


workspace(name = "cloud-cpp-mini-client")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
git_repository(
    name = "google_cloud_cpp",
    remote = "https://github.com/googleapis/google-cloud-cpp.git",
    branch = "main",
)

# Alternate to above, for grabbing specific client snapshots
#load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
#http_archive(
#    name = "google_cloud_cpp",
#    sha256 = "ac93ef722d08bfb220343bde2f633c7c11f15e34ec3ecd0a57dbd3ff729cc3a6",
#    strip_prefix = "google-cloud-cpp-2.5.0",
#    url = "https://github.com/googleapis/google-cloud-cpp/archive/v2.5.0.tar.gz",
#)

# Load indirect dependencies due to
#     https://github.com/bazelbuild/bazel/issues/1943
load("@google_cloud_cpp//bazel:google_cloud_cpp_deps.bzl", "google_cloud_cpp_deps")
google_cloud_cpp_deps()

load("@com_google_googleapis//:repository_rules.bzl", "switched_rules_by_language")
switched_rules_by_language(
    name = "com_google_googleapis_imports",
    cc = True,
    grpc = True,
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()

# For capturing p50, p99, etc.
git_repository(
    name = "streaming-percentiles",
    remote = "https://github.com/BrandonY/streaming-percentiles.git",
    # using a fork until pull request for supporting Bazel 6 is submitted.
    #remote = "https://github.com/sengelha/streaming-percentiles.git",
    branch = "main",
)

# The cloud storage C++ repo depends on a cURL target named "com_github_curl_curl".
# The prometheus-cpp repo depends on a separate cURL target named "com_github_curl"
# This declares "@com_github_curl" as an alias of "@com_github_curl_curl" in order
# to trick prometheus-cpp into not importing a second verison of curl.
new_local_repository(
  name = "com_github_curl",
  path = "fake_curl",
  build_file = "fake_curl/BUILD.com_github_curl",
)


load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")
http_archive(
    name = "com_github_jupp0r_prometheus_cpp",
    strip_prefix = "prometheus-cpp-master",
    urls = ["https://github.com/jupp0r/prometheus-cpp/archive/master.zip"],
)
load("@com_github_jupp0r_prometheus_cpp//bazel:repositories.bzl", "prometheus_cpp_repositories")
prometheus_cpp_repositories()

# For flags
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "com_google_absl",
  urls = ["https://github.com/abseil/abseil-cpp/archive/98eb410c93ad059f9bba1bf43f5bb916fc92a5ea.zip"],
  strip_prefix = "abseil-cpp-98eb410c93ad059f9bba1bf43f5bb916fc92a5ea",
)

# absl dependency
http_archive(
  name = "bazel_skylib",
  urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
  sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
)