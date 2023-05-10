include $(TOPDIR)/rules.mk

PKG_NAME:=buggy
PKG_VERSION:=1.0
PKG_RELEASE:=1

SOURCE_DIR:=./src

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=examples
	CATEGORY:=Examples
	TITLE:=Buggy
endef


define Package/$(PKG_NAME)/description
  	All bugs shall be eliminated from this Earth.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -ggdb -o $(PKG_BUILD_DIR)/bug1.o -c $(PKG_BUILD_DIR)/bug1.c
	$(TARGET_CC) $(TARGET_CFLAGS) -ggdb -o $(PKG_BUILD_DIR)/bug2.o -c $(PKG_BUILD_DIR)/bug2.c
	$(TARGET_CC) $(TARGET_CFLAGS) -ggdb -o $(PKG_BUILD_DIR)/bug3.o -c $(PKG_BUILD_DIR)/bug3.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/bug1 $(PKG_BUILD_DIR)/bug1.o
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/bug2 $(PKG_BUILD_DIR)/bug2.o
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/bug3 $(PKG_BUILD_DIR)/bug3.o
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bug1 $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bug2 $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bug3 $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))


