include $(TOPDIR)/rules.mk

PKG_NAME:=lidar
PKG_VERSION:=1.0
PKG_RELEASE:=1

SOURCE_DIR:=./src

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=projekt
	CATEGORY:=projekt
	DEPENDS:=+libstdcpp +libc
	TITLE:=Lidar
endef

define Package/$(PKG_NAME)/description
	Lidar - proximity sensor operating in an area
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
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
