FROM ts-dockerhub.lsst.org/deploy-crio:c0045 AS setup

# Avro (C++) and libschemaregistry are provided by the base image at
# LSST_SAL_PREFIX (/opt/lsst/tssw/ts_sal); SAL generated code links against
# them (OSW-2238). No local avro/libserdes builds: a second copy of those
# libraries on the include/link paths is what caused the version clashes
# this Dockerfile previously worked around.

USER saluser
ARG XML_BRANCH=develop
WORKDIR /home/saluser

ARG KAFKA_HOST=
ARG KAFKA_BROKER_PORT=
ARG SCHEMA_REGISTRY_URI=

RUN [ -z $KAFKA_HOST -o -z $KAFKA_BROKER_PORT ] || echo >> .crio_setup.sh -e \
\\n\
export LSST_KAFKA_BROKER_ADDR="${KAFKA_HOST}:${KAFKA_BROKER_PORT}"\\n\
export LSST_KAFKA_HOST=${KAFKA_HOST}\\n\
export LSST_KAFKA_BROKER_PORT=${KAFKA_BROKER_PORT}

RUN [ -z $SCHEMA_REGISTRY_URI ] || echo >> .crio_setup.sh -e \
\\n\
export LSST_SCHEMA_REGISTRY_URL=${SCHEMA_REGISTRY_URI}

RUN echo >> .crio_setup.sh -e \
export PKG_CONFIG_PATH=\$PKG_CONFIG_PATH:/opt/lsst/software/stack/miniconda/share/pkgconfig\\n\
export LIBSCHEMAREGISTRY_VCPKG_LIB=/opt/vcpkg/installed/x64-linux/lib

RUN source ~/.crio_setup.sh && cd $TS_XML_DIR \
    && git fetch && git checkout $XML_BRANCH && git pull \
    && pip install .

RUN source ~/.crio_setup.sh \
    && MAKEFLAGS="-j$(nproc)" salgeneratorKafka generate cpp MTM1M3 \
    && MAKEFLAGS="-j$(nproc)" G_LIB=/opt/vcpkg/installed/x64-linux/lib salgeneratorKafka generate cpp MTMount

ARG cRIO_CPP=v1.16.1
ARG M1M3_SUPPORT=develop
ARG TARGET=simulator

FROM setup AS build

RUN source ~/.crio_setup.sh \
    && cd ts_cRIOcpp && git fetch && git checkout $cRIO_CPP \
    && make clean && make -j$(nproc)

RUN source ~/.crio_setup.sh \
    && git clone --branch $M1M3_SUPPORT https://github.com/lsst-ts/ts_m1m3support \
    && cd ts_m1m3support \
    && PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/opt/lsst/software/stack/miniconda/share/pkgconfig LIBSCHEMAREGISTRY_VCPKG_LIB=/opt/vcpkg/installed/x64-linux/lib make -j$(nproc) SIMULATOR=1 $TARGET

SHELL ["/bin/bash", "-lc"]
