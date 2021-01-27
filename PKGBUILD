_reponame=qClash
pkgname=qClash
pkgver=0.1.3
pkgrel=1
arch=('x86_64')
pkgdesc='Clash Qt Gui Client'
license=('GPL')
depends=('qt5-base' 'qt5-sebsockets' 'qt5-svg' 'yaml-cpp')
makedepends=('cmake' 'ninja' 'qt5-tools')
url=""https://github.com/josephpei/${_reponame}
source=()
# source=("git+$url.git")
sha512sums=(SKIP)

prepare() {
    cd "$startdir"
    mkdir build
    # cd $srcdir
    # ln -s SingleApplication-3.2.0/* ${_reponame}-${pkgver}/src/third_party/singleapplication/

    # cd qClash
    # git submodule init
    # git config
    # git submodule update
}

build() {
    # cd "$srcdir/${PROJECT_DIR_NAME:-$_reponame-$pkgver}"
    # cd qClash
    cd "$startdir/build"

    cmake \
        -G Ninja \
        -DCMAKE_BUILD_TYPE:STRING='Release' \
        -DCMAKE_INSTALL_PREFIX:PATH='/opt' \
        ..
    ninja
}

package() {
    # cd "$srcdir/${PROJECT_DIR_NAME:-$_reponame-$pkgver}"
    cd "$startdir/build"
    DESTDIR='/opt' ninja install
}