#pragma once

#include "envoy/config/common/dynamic_forward_proxy/v2alpha/dns_cache.pb.h"
#include "envoy/event/dispatcher.h"
#include "envoy/singleton/manager.h"
#include "envoy/thread_local/thread_local.h"

namespace Envoy {
namespace Extensions {
namespace Common {
namespace DynamicForwardProxy {

/**
 * fixfix
 */
class DnsCache {
public:
  /**
   * fixfix
   */
  class LoadDnsCacheCallbacks {
  public:
    virtual ~LoadDnsCacheCallbacks() = default;

    /**
     * fixfix
     */
    virtual void onLoadDnsCacheComplete() PURE;
  };

  /**
   * fixfix
   */
  class LoadDnsCacheHandle {
  public:
    virtual ~LoadDnsCacheHandle() = default;
  };

  using LoadDnsCacheHandlePtr = std::unique_ptr<LoadDnsCacheHandle>;

  /**
   * fixfix
   */
  class AddUpdateCallbacksHandle {
  public:
    virtual ~AddUpdateCallbacksHandle() = default;
  };

  using AddUpdateCallbacksHandlePtr = std::unique_ptr<AddUpdateCallbacksHandle>;

  virtual ~DnsCache() = default;

  /**
   * fixfix
   */
  virtual LoadDnsCacheHandlePtr loadDnsCache(absl::string_view host,
                                             LoadDnsCacheCallbacks& callbacks) PURE;

  /**
   * fixfix
   */
  using UpdateCallbacks = std::function<void()>;
  virtual AddUpdateCallbacksHandlePtr addUpdateCallbacks(const UpdateCallbacks& callbacks) PURE;

  /**
   * fixfix
   */
  // fixfixvirtual const std::list<Upstream::HostConstSharedPtr>& hosts() PURE;
};

using DnsCacheSharedPtr = std::shared_ptr<DnsCache>;

/**
 * fixfix
 */
class DnsCacheManager {
public:
  virtual ~DnsCacheManager() = default;

  /**
   * fixfix
   */
  virtual DnsCacheSharedPtr getCache(
      const envoy::config::common::dynamic_forward_proxy::v2alpha::DnsCacheConfig& config) PURE;
};

using DnsCacheManagerSharedPtr = std::shared_ptr<DnsCacheManager>;

/**
 * fixfix
 */
DnsCacheManagerSharedPtr getCacheManager(Singleton::Manager& manager,
                                         Event::Dispatcher& main_thread_dispatcher,
                                         ThreadLocal::SlotAllocator& tls);

} // namespace DynamicForwardProxy
} // namespace Common
} // namespace Extensions
} // namespace Envoy
