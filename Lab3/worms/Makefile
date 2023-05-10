include $(TOPDIR)/rules.mk

PKG_NAME:=worms
PKG_VERSION:=1.0
PKG_RELEASE:=1

SOURCE_DIR:=./src

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=examples
	CATEGORY:=Examples
	DEPENDS:=+libncurses
	TITLE:=Worms
endef

define Package/$(PKG_NAME)/description
	It's called worms but it's snake.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/worms.o -c $(PKG_BUILD_DIR)/worms.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/helper.o -c $(PKG_BUILD_DIR)/helper.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/main.o -c $(PKG_BUILD_DIR)/main.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/worms $(PKG_BUILD_DIR)/main.o $(PKG_BUILD_DIR)/helper.o $(PKG_BUILD_DIR)/worms.o -lncurses
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/worms $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
