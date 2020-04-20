#!/bin/bash

now() {
    date +%x.%H:%M:%S:%N
}

sleep_seconds()
{
    seconds=${1:-15}
    echo -e "\n`now` Sleep for ${seconds}"
    sleep ${seconds}
}

show_metrics() {
    echo -e "\n### `now`: Show metrics ### "
    echo "/clusters cx-active:"
    curl http://localhost:8001/clusters 2>/dev/null | grep 'cx_active' | grep service1
    echo "/stats/prometheus cx-active:"
    curl http://localhost:8001/stats/prometheus 2>/dev/null | grep 'upstream_cx_active' | grep service1
    echo "netstat -tup -W:"
    netstat -tup -W | grep front-proxy_service1
}

make_some_requests() {
    echo -e "\n### `now`: Send some requests ### "
    wrk -t12 -c400 -d10s http://localhost:80/service/1
}

make_hot_restart() {
    pid=`pgrep -nf hot-restarter.py`
    echo -e "\n### `now`: making hot-restart, sending sighup to pid: ${pid} ### "
    /bin/kill -SIGHUP ${pid}
}

check_envoys_uptime_and_wait_for_parent_to_die() {
    echo -e "\n### `now`: Checking envoys exists  ### "
    ps -fC envoy | cat
    curl -s  http://localhost:8001/server_info | jq '{"restart_epoch": .command_line_options.restart_epoch, "uptime_current_epoch": .uptime_current_epoch, "uptime_all_epochs": .uptime_all_epochs}'
    if [[ `ps -fC envoy | cat | grep envoy | wc -l` -eq 2 ]]; then
        echo "Parent still exist"
        sleep_seconds 3
        check_envoys_uptime_and_wait_for_parent_to_die
    fi
}


show_metrics
make_some_requests
show_metrics

make_hot_restart
check_envoys_uptime_and_wait_for_parent_to_die
show_metrics

make_some_requests
show_metrics
sleep_seconds

make_hot_restart
check_envoys_uptime_and_wait_for_parent_to_die
show_metrics
