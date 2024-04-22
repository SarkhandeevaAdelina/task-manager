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

### Возможные проблемы

```
git clone https://github.com/mxe/mxe.git
Клонирование в «mxe»...
remote: Enumerating objects: 56805, done.
remote: Counting objects: 100% (16987/16987), done.
remote: Compressing objects: 100% (677/677), done.
error: 821 bytes of body are still expected37 МиБ | 92.00 КиБ/с 
fetch-pack: unexpected disconnect while reading sideband packet
fatal: неожиданный конец файла
fatal: fetch-pack: invalid index-pack output
```

Ошибка, которую вы видите, может быть вызвана различными причинами, такими как проблемы с сетью, прерывание соединения или ошибки на сервере GitHub.

Попробуйте выполнить следующие действия, чтобы решить проблему:

1) Убедитесь, что ваше интернет-соединение работает стабильно.
2) Попробуйте клонировать репозиторий еще раз, используя протокол HTTPS:
```
git clone https://github.com/mxe/mxe.git
```
3) Если проблема не решена, попробуйте использовать протокол SSH. Для этого вам нужно сначала сгенерировать SSH-ключ и добавить его в ваш аккаунт GitHub. Инструкции по настройке SSH-ключа можно найти здесь: https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent
4) После настройки SSH-ключа вы можете клонировать репозиторий, используя протокол SSH:
```
git clone git@github.com:mxe/mxe.git
```
Если вы все еще испытываете проблемы, попробуйте увеличить буфер Git для HTTPS-соединения. Для этого выполните следующую команду перед клонированием репозитория:
```
git config --global http.postBuffer 157286400
```
Эта команда устанавливает размер буфера для HTTPS-соединения в 150 МБ.

После выполнения этих действий попробуйте еще раз клонировать репозиторий. Если проблема не исчезнет, сообщите об этом администраторам GitHub или обратитесь за дополнительной помощью.

## Windows
### Установка

Этот раздел дорабатывается.
