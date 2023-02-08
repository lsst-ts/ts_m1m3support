FROM lsstts/develop-env:c0028.002 as crio-develop

ARG XML_BRANCH=develop

USER root
RUN chmod a+rwX -R /home/saluser/
USER saluser

ARG XML_BRANCH=main

WORKDIR /home/saluser

RUN source ~/.setup.sh \
    && mamba install -y readline yaml-cpp boost-cpp catch2 spdlog \
    && echo > .crio_setup.sh -e \
echo "Configuring cRIO development environment" \\n\
export SHELL=bash \\n\
source /home/saluser/.setup_salobj.sh \\n\
export PATH=\$CONDA_PREFIX/bin:\$PATH \\n\
export LIBS="-L\$CONDA_PREFIX/lib" \\n\
export CPP_FLAGS="-I\$CONDA_PREFIX/include" \\n

# temporary upgrade ts_sal, needed for current develop ts_xml (which changed paths)
RUN source ~/.crio_setup.sh && cd repos/ts_sal \
    && git fetch && git checkout develop && git pull

RUN source ~/.crio_setup.sh && cd $TS_XML_DIR \
    && git fetch && git checkout $XML_BRANCH && git pull \
    && salgenerator generate cpp MTM1M3 \
    && salgenerator generate cpp MTMount

FROM crio-develop

ARG cRIO_CPP=v1.5.1
ARG M1M3_SUPPORT=develop
ARG TARGET=simulator

RUN cd repos && git clone --branch $cRIO_CPP https://github.com/lsst-ts/ts_cRIOcpp
RUN source ~/.crio_setup.sh && cd repos/ts_cRIOcpp && make

RUN cd repos && git clone --branch $M1M3_SUPPORT https://github.com/lsst-ts/ts_m1m3support
RUN source ~/.crio_setup.sh && cd repos/ts_m1m3support && make $TARGET

SHELL ["/bin/bash", "-lc"]
