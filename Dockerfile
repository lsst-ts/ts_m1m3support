FROM lsstts/develop-env:develop AS crio-develop

# Avro (C++) and libschemaregistry are provided by the base image at
# LSST_SAL_PREFIX (/opt/lsst/tssw/ts_sal); SAL generated code links against
# them (OSW-2238). No local avro/libserdes builds: a second copy of those
# libraries on the include/link paths is what caused the version clashes
# this Dockerfile previously worked around.

USER saluser
ARG XML_BRANCH=develop
WORKDIR /home/saluser

RUN source ~/.setup_salobj.sh \
    && mamba install -y readline yaml-cpp catch2 spdlog texlive-core ghostscript 

RUN source ~/.setup_salobj.sh \
    && echo > .crio_setup.sh -e \
echo "Configuring cRIO development environment" \\n\
export SAL_HOME=/opt/lsst/tssw/ts_sal/lsstsal\\n\
export SAL_WORK_DIR=/home/saluser/repos/ts_sal/test\\n\
export LSST_KAFKA_PREFIX=sal\\n\
export LSST_KAFKA_SECURITY_MECHANISM=SCRAM-SHA-512\\n\
export LSST_KAFKA_CLASSDIR=/opt/lsst/tssw/ts_sal/lib\\n\
export LSST_KAFKA_HOST=broker\\n\
export LSST_KAFKA_IP=\\n\
export LSST_KAFKA_LOCAL_SCHEMAS=/opt/lsst/tssw/ts_sal/test\\n\
export LSST_KAFKA_BROKER_PORT=29092\\n\
export LSST_KAFKA_BROKER_ADDR=broker:29092\\n\
export LSST_KAFKA_SECURITY_PROTOCOL=SASL_SSL\\n\
export AVRO_RELEASE=1.11.3\\n\
export LSST_TOPIC_SUBNAME=sal\\n\
\\n\
export LSST_KAFKA_BROKER_ADDR="broker:29092"\\n\
export LSST_SCHEMA_REGISTRY_URL="http://schema-registry:8081"\\n\
\\n\
export LSST_KAFKA_HOST=${LSST_KAFKA_BROKER_ADDR%:*}\\n\
export LSST_KAFKA_BROKER_PORT=${LSST_KAFKA_BROKER_ADDR##*:}\\n\
export LSST_KAFKA_PREFIX=sal\\n\
export LSST_KAFKA_LOCAL_SCHEMAS=$SAL_WORK_DIR\\n\
\\n\
export TS_CONFIG_OCS_DIR=/lsst/ts_config_ocs/\\n\
\\n\
export LIBRARY_PATH=/opt/lsst/tssw/ts_sal/lib\\n\
\\n\
source /home/saluser/.setup_salobj.sh \
source /opt/lsst/tssw/ts_sal/setupKafka.env \
\\n\
setup ts_idl -t current \\n\
setup ts_sal -t current \\n\
setup ts_salobj -t current \\n\
setup ts_xml -t current \\n\
\\n\
export SAL_DIR=/opt/lsst/tssw/ts_sal/lsstsal/scripts

RUN source ~/.crio_setup.sh \
    && cd $TS_XML_DIR \
    && git fetch && git checkout $XML_BRANCH && git pull \
    && pip install .

RUN source ~/.crio_setup.sh \
    && salgeneratorKafka generate cpp MTM1M3 \
    && salgeneratorKafka generate cpp MTMount

ARG cRIO_CPP=v1.16.0
ARG M1M3_SUPPORT=develop
ARG TARGET=simulator

RUN source ~/.crio_setup.sh \
    && git clone --branch $cRIO_CPP https://github.com/lsst-ts/ts_cRIOcpp \
    && cd ts_cRIOcpp && make

RUN source ~/.crio_setup.sh \
    && git clone --branch $M1M3_SUPPORT https://github.com/lsst-ts/ts_m1m3support \
    && cd ts_m1m3support && make $TARGET

SHELL ["/bin/bash", "-lc"]
