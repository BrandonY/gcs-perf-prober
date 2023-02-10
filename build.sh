#!/bin/bash

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

# exit when any command fails
set -e

bazel build -c opt :gcs_perf_prober

# bazel-bin is a symlink out of the directory, so Docker doesn't like it.
rm -rf target
mkdir -p target
mv bazel-bin/gcs_perf_prober target/

# Login to Could container stuff
gcloud auth print-access-token | sudo docker login -u oauth2accesstoken --password-stdin https://us-central1-docker.pkg.dev

# Build the image locally
docker build -t us-central1-docker.pkg.dev/gcs-grpc-team-testing/my-repository/gcs-perf-prober:latest .

# Push the image to google cloud
docker push us-central1-docker.pkg.dev/gcs-grpc-team-testing/my-repository/gcs-perf-prober:latest

echo "Success. To start one of these, use kubectl apply. e.g.:  kubectl apply -f deployments/prod-us-east1-100k-object-read.yaml"
echo "To stop it, kubectl get pods to get the ID, then kubectl kill."