# Copyright 2023 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

FROM ubuntu:22.10

# Curl necessary for ca-certificates
# net-tools for online diagnostics
RUN apt-get -qq update
RUN apt-get install -y -q curl apt-transport-https net-tools

WORKDIR /usr/perf-prober
COPY /target/gcs_perf_prober .
COPY run_scripts/docker_launch.sh .


CMD "/usr/perf-prober/docker_launch.sh" 