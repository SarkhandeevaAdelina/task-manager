# Менеджер задач
## Ubuntu 
### Установка

1) Откройте терминал и обновите список пакетов:
```
sudo apt-get update
```
2) Установите необходимые зависимости:
```
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev libx11-xcb-dev libxcb-glx0-dev qtbase5-dev qtdeclarative5-dev qttools5-dev qttools5-dev-tools qtcreator
```
3) Скачайте репозиторий (либо zip-файлом, либо через команду в терминале).
```
git clone https://github.com/SarkhandeevaAdelina/task-manager.git
```
4) Запустите Qt Creator:
```
qtcreator
``` 
5) Откройте файл CMakeLists.txt.

### Кросс-компиляция (компиляция проекта для Windows с платформы Linux

Для кросс-компиляции будем использовать MXE.

1) Скачайте зависимости MXE:
```
apt-get install \
    autoconf \
    automake \
    autopoint \
    bash \
    bison \
    bzip2 \
    flex \
    g++ \
    g++-multilib \
    gettext \
    git \
    gperf \
    intltool \
    libc6-dev-i386 \
    libgdk-pixbuf2.0-dev \
    libltdl-dev \
    libgl-dev \
    libpcre3-dev \
    libssl-dev \
    libtool-bin \
    libxml-parser-perl \
    lzip \
    make \
    openssl \
    p7zip-full \
    patch \
    perl \
    python3 \
    python3-distutils \
    python3-mako \
    python3-pkg-resources \
    python-is-python3 \
    ruby \
    sed \
    unzip \
    wget \
    xz-utils
```

2) Скачайте MXE.
```
git clone https://github.com/mxe/mxe.git
```
3) Соберите Qt 5 для Windows:
```
cd mxe && make qtbase
```
Сначала будут собраны зависимости и инструменты для кросс-сборки; это займет меньше часа на быстрой машине с приличным доступом в интернет.

Благодаря новой модульной природе Qt 5, различные основные компоненты Qt теперь находятся в разных tarball'ах. Выбранный выше qtbase должен дать вам достаточно функциональности для запуска обычных GUI-приложений, а это все, что мне было нужно для моего собственного (небольшого) приложения.

Если вы хотите собрать весь Qt 5, вам нужно будет запустить make qt5 (вместо make qtbase). Обратите внимание, что это займет гораздо больше времени, поэтому убедитесь, что вам нужна дополнительная функциональность.

4) Перейдите в каталог вашего приложения и запустите инструмент генератора Qt Makefile:
```
<mxe root>/usr/bin/i686-w64-mingw32.static-qmake-qt5
```
5) Соберите свой проект:
```
make
```
Вы должны найти двоичный файл в каталоге ./release:
```
wine release/foo.exe
```

## Windows
### Установка

Этот раздел дорабатывается.
