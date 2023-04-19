# Instalacja Open WRT

Wykrozystaliśmy prekomiplowaną wersję OpenWRT dostępną na stronie openwrt.org. Obraz jądra wraz z system plików pobraliśmy i rozpakowaliśmy przy pomocy poleceń:

> curl https://downloads.openwrt.org/releases/21.02.1/targets/armvirt/64/openwrt-21.02.1-armvirt-64-Image > Image

> curl https://downloads.openwrt.org/releases/21.02.1/targets/armvirt/64/openwrt-21.02.1-armvirt-64-rootfs-ext4.img.gz > rootfs.ext4.gz

> gunzip rootfs.ext4.gz 

# Odpalenie OpenWRT na Qemu

Uruchomiliśmy Qemu z podanymi niżej parametrami:

```bash
qemu-system-aarch64 -nographic \
                    -M virt \
                    -m 2048 \
                    -cpu cortex-a72 \
                    -smp 4 \
                    -append "root=/dev/vda console=ttyAMA0" \
                    -kernel Image \
                    -drive file=rootfs.ext4,format=raw \
                    -nic user,hostfwd=tcp::8889-:80,hostfwd=tcp::8888-:22
```

Następnie zmodyfikowaliśmy konfigurację sieci do przesłanaia plików na OpenWRT w pliku network:

```bash
config interface 'lan'
        option device 'br-lan'
        option proto 'dhcp'
        option ipaddr '192.168.1.1'
        option netmask '255.255.255.0'
        option ip6assign '60'
```

Następnie przeładowalismy konfigurację i usnęliiśmy zbiór uruchamiajacy serwer DNS/DHCP dnsmasq.

# Instalacja Pythona i odpalenie programu

Wykrozystaliśmy kolejno komendy:
> opkg update  \
> opkg install python3 

Następnie korzystając z serwera http i polecenia wget przekopiowaliśmy na maszynę wirtualną plik PWM.py z kodem generującym sygnał PWM. Po odpaleniu pliku w konsoli wyświetlane są generowane sygnały PWM.