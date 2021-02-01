_reponame=qClash
pkgname=qClash
pkgver=0.2.0
pkgrel=1
arch=('x86_64')
pkgdesc='Clash Qt Gui Client'
license=('GPL')
depends=('qt5-base' 'qt5-websockets' 'qt5-svg' 'yaml-cpp')
makedepends=('cmake' 'ninja' 'qt5-tools')
url=""https://github.com/josephpei/${_reponame}
source=()
# source=("git+$url.git")
sha512sums=()

prepare() {
    cd "$startdir"
    mkdir -p build
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
        -DCMAKE_BUILD_TYPE:STRING='Release' \
        ..
    make -j8
}

package() {
    # cd "$srcdir/${PROJECT_DIR_NAME:-$_reponame-$pkgver}"
    cd "$startdir/build"
    DESTDIR="$pkgdir/" make install
}