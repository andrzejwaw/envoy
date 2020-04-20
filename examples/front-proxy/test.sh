#!/bin/bash

show_metrics() {
    echo " ### Show metrics ### "
    echo "/clusters cx-active:"
    curl http://localhost:8001/clusters 2>/dev/null | grep 'cx_active' | grep service1
    echo "/stats/prometheus cx-active:"
    curl http://localhost:8001/stats/prometheus 2>/dev/null | grep 'upstream_cx_active' | grep service1
    echo "netstat -a:"
    netstat -a
}

make_some_requests() {
    echo "vsend some requests ### "
    wrk -t12 -c400 -d10s http://localhost:8000/service/1
}

make_hot_restart() {
    pid=`pgrep -nf hot-restarter.py`
    echo " ### making hot-restart ${pid} ### "
    /bin/kill -SIGHUP ${pid}
}

check_envoys_uptime() {
    echo " ### Checking envoys exists  ### "
    ps aux | grep envoy
    curl http://localhost:8001/server_info | grep uptime
}

show_metrics
make_some_requests
show_metrics
make_hot_restart
check_envoys_uptime
sleep 15
check_envoys_uptime
show_metrics
make_some_requests
show_metrics