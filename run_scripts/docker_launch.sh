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

echo "Hi, I'm Brandon's docker startup script"

echo "You want me to run $SCENARIO in region $REGION in universe $UNIVERSE, for $OPERATION operation"

if [[ -z "${SCENARIO}" ]]; then
  echo "You must set SCENARIO variable"
  exit
fi

if [[ -z "${REGION}" ]]; then
  echo "You must set REGION variable"
  exit
fi

if [[ -z "${UNIVERSE}" ]]; then
  echo "You must set UNIVERSE variable"
  exit
fi

if [[ -z "${OPERATION}" ]]; then
  echo "You must set OPERATION variable"
  exit
fi

if [[ "${OPERATION}" == *"Write"* ]] && [[ -z "${WRITE_LENGTH}" ]]; then
    echo "You must set WRITE_LENGTH for WriteObject operations."
    exit
fi

if [[ "preprod" == "$UNIVERSE" ]]; then
	BUCKET_PREFIX="gcs-grpc-team-preprod-perf"
else
	BUCKET_PREFIX="gcs-grpc-team-perf-testing"
fi

export BUCKET="${BUCKET_PREFIX}-${REGION}-${SCENARIO}"
echo "Running on bucket '$BUCKET'"

./gcs_perf_prober --universe=$UNIVERSE --api=json --scenario=$SCENARIO --region=$REGION --operation=$OPERATION --object_name=100K --push_to_prometheus --run_duration=1m --write_length=$WRITE_LENGTH &
./gcs_perf_prober --universe=$UNIVERSE --api=grpc --scenario=$SCENARIO --region=$REGION --operation=$OPERATION --object_name=100K --push_to_prometheus --run_duration=1m --write_length=$WRITE_LENGTH

# And start JSON as well.