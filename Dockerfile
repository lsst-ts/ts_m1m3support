FROM lsstts/develop-env:c0024.007 as crio-develop

ARG XML_BRANCH=develop

USER root
RUN chmod a+rwX -R /home/saluser/
USER saluser

ARG XML_BRANCH=main

WORKDIR /home/saluser/repos/ts_xml
RUN git fetch && git checkout $XML_BRANCH

WORKDIR /home/saluser

RUN source .setup.sh \
    && conda install -y readline yaml-cpp boost-cpp catch2 \
    && echo > .crio_setup.sh -e \
echo "Configuring cRIO development environment" \\n\
source /home/saluser/.setup_salobj.sh \\n\
export PATH=\$CONDA_PREFIX/bin:\$PATH \\n\
export LIBS="-L\$CONDA_PREFIX/lib" \\n\
export CPP_FLAGS="-I\$CONDA_PREFIX/include" \\n\
export PKG_CONFIG_PATH="\$CONDA_PREFIX/lib/pkgconfig" \\n

RUN source .crio_setup.sh && cd repos/ts_xml/sal_interfaces \
    && git fetch && git checkout $XML_BRANCH \
    && touch $TS_XML_DIR/VERSION \
    && cp *.xml MTM1M3/*.xml MTMount/*.xml $SAL_WORK_DIR \
    && salgenerator generate cpp MTM1M3 \
    && salgenerator generate cpp MTMount

FROM crio-develop

ARG cRIO_CPP=v1.1.0
ARG M1M3_SUPPORT=develop
ARG TARGET=simulator

RUN cd repos && git clone --branch $cRIO_CPP https://github.com/lsst-ts/ts_cRIOcpp
RUN source .crio_setup.sh && cd repos/ts_cRIOcpp && make

RUN cd repos && git clone --branch $M1M3_SUPPORT https://github.com/lsst-ts/ts_m1m3support
RUN source .crio_setup.sh && cd repos/ts_m1m3support && make $TARGET

SHELL ["/bin/bash", "-lc"]
