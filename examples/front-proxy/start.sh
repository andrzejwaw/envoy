#!/bin/bash
/usr/local/bin/envoy -l debug -c /etc/front-envoy.yaml --service-cluster front-proxy --service-node front-proxy-id --base-id 1 --restart-epoch $RESTART_EPOCH --parent-shutdown-time-s 15 --drain-time-s 10
