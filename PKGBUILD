_reponame=qClash
pkgname=qClash
pkgver=0.1.3
pkgrel=1
arch=('x86_64')
pkgdesc='Clash Qt Gui Client'
license=('GPL')
depends=('qt5-base' 'yaml-cpp')
makedepends=('cmake' 'ninja' 'qt5-tools')
url=""https://github.com/josephpei/${_reponame}
source=("${pkgname}-${pkgver}.tar.gz::https://github.com/josephpei/${_reponame}/archive/v${pkgver}.tar.gz"
    "https://github.com/itay-grudev/SingleApplication/archive/v3.2.0.tar.gz")
# source=("git+$url.git")
sha512sums=(SKIP SKIP)

prepare() {
    cd $srcdir
    ln -s SingleApplication-3.2.0/* ${_reponame}-${pkgver}/src/third_party/singleapplication/
    # cd qClash
    # git submodule init
    # git config
    # git submodule update
}

build() {
    cd "$srcdir/${PROJECT_DIR_NAME:-$_reponame-$pkgver}"
    # cd qClash

    cmake \
        -G Ninja \
        -DCMAKE_BUILD_TYPE:STRING='Release' \
        -DCMAKE_INSTALL_PREFIX:PATH='/opt' \
        .
    ninja
}

package() {
    cd "$srcdir/${PROJECT_DIR_NAME:-$_reponame-$pkgver}"
    DESTDIR='/opt' ninja install
}