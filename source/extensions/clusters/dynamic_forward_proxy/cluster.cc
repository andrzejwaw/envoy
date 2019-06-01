#include "extensions/clusters/dynamic_forward_proxy/cluster.h"

namespace Envoy {
namespace Extensions {
namespace Clusters {
namespace DynamicForwardProxy {

Cluster::Cluster(
    const envoy::api::v2::Cluster& cluster,
    const envoy::config::cluster::dynamic_forward_proxy::v2alpha::ClusterConfig& config,
    Runtime::Loader& runtime, Singleton::Manager& singleton_manager,
    Event::Dispatcher& main_thread_dispatcher, ThreadLocal::SlotAllocator& tls,
    Server::Configuration::TransportSocketFactoryContext& factory_context,
    Stats::ScopePtr&& stats_scope, bool added_via_api)
    : Upstream::BaseDynamicClusterImpl(cluster, runtime, factory_context, std::move(stats_scope),
                                       added_via_api),
      dns_cache_manager_(Common::DynamicForwardProxy::getCacheManager(singleton_manager,
                                                                      main_thread_dispatcher, tls)),
      dns_cache_(dns_cache_manager_->getCache(config.dns_cache_config())),
      update_callbacks_handle_(dns_cache_->addUpdateCallbacks([this] { onDnsCacheUpdate(); })) {}

void Cluster::onDnsCacheUpdate() {
  ASSERT(false); // fixfix
}

std::pair<Upstream::ClusterImplBaseSharedPtr, Upstream::ThreadAwareLoadBalancerPtr>
ClusterFactory::createClusterWithConfig(
    const envoy::api::v2::Cluster& cluster,
    const envoy::config::cluster::dynamic_forward_proxy::v2alpha::ClusterConfig& proto_config,
    Upstream::ClusterFactoryContext& context,
    Server::Configuration::TransportSocketFactoryContext& socket_factory_context,
    Stats::ScopePtr&& stats_scope) {
  return std::make_pair(std::make_shared<Cluster>(cluster, proto_config, context.runtime(),
                                                  context.singletonManager(), context.dispatcher(),
                                                  context.tls(), socket_factory_context,
                                                  std::move(stats_scope), context.addedViaApi()),
                        nullptr);
}

REGISTER_FACTORY(ClusterFactory, Upstream::ClusterFactory);

} // namespace DynamicForwardProxy
} // namespace Clusters
} // namespace Extensions
} // namespace Envoy
