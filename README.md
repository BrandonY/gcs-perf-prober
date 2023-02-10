# GCS Performance Prober

This repository contains a small tool for measuring and monitoring
the performance of Google Cloud Storage's various APIs against each
other. It's intended to be used within Google Cloud and supports
reporting the results to a Prometheus database.

Measuring/monitoring GCS performance on an ongoing basis is potentially
expensive and not something almost anyone would want to do. Be careful
using a tool like this.

## How to use

1. Clone it from GitHub.
2. Build it: `bazel build :gcs-perf-prober`
3. Run some test scenario:

``` shell
    bazel run :gcs-perf-prober -- \
        --universe=prod --api=json --scenario=small-reads \
        --region=us-central1 --operation=ReadObject \
        --object_name=100K --run_duration=1m
```

## Contributing

You probably don't want to? But pull requests are fine.
