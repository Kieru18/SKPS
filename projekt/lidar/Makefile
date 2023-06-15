include $(TOPDIR)/rules.mk

PKG_NAME:=lidarRPi
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=lidarRPi
  CATEGORY:=lidarRPi
  DEPENDS:=+libstdcpp +libc
  TITLE:=lidarRPi
endef

define Package/$(PKG_NAME)/description
  Lidar - proximity sensor operating in an area
endef

define Build/Compile
	$(MAKE) -C "$(PKG_BUILD_DIR)/VL53L0X_rasp" clean
	$(MAKE) -C "$(PKG_BUILD_DIR)" clean
	$(MAKE) -C $(PKG_BUILD_DIR) \
			CXX="$(TARGET_CXX)" \
			CC="$(TARGET_CC)" \
			CXXFLAGS="$(TARGET_CXXFLAGS)" \
			CFLAGS="$(TARGET_CFLAGS)" \
			LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin 
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lidar $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
