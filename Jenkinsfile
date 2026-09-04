#!/usr/bin/env groovy

properties(
    [
    buildDiscarder
        (logRotator (
            artifactDaysToKeepStr: '',
            artifactNumToKeepStr: '',
            daysToKeepStr: '14',
            numToKeepStr: ''
        ) ),
    disableConcurrentBuilds(),
    parameters
        ( [
            booleanParam(defaultValue: false, description: 'Adds --no-cache to Docker build command', name: 'noCache'),
            booleanParam(defaultValue: false, description: 'Calls make clean before building the code', name: 'clean')
        ] )
    ]
)

node {
    def SALUSER_HOME = "/home/saluser"
    def BRANCH = (env.CHANGE_BRANCH != null) ? env.CHANGE_BRANCH : env.BRANCH_NAME
    def SAME_CRIO_BRANCH = ["main"]
    def CRIO_BRANCH=(BRANCH in SAME_CRIO_BRANCH) ? BRANCH : "develop"

    stage('Cloning sources')
    {
        dir("ts_m1m3support") {
            checkout scm
        }
    }

    stage('Setup dev container')
    {
        M1M3sim = docker.build(
            "lsstts/mtm1m3_sim:" + env.BRANCH_NAME.replace("/", "_"),
            "--target setup --build-arg XML_BRANCH=main "
            + "--build-arg KAFKA_HOST=$LSST_KAFKA_HOST --build-arg KAFKA_BROKER_PORT=$LSST_KAFKA_BROKER_PORT "
            + "--build-arg SCHEMA_REGISTRY_URI=$LSST_SCHEMA_REGISTRY_URL "
            + "--build-arg cRIO_CPP=$CRIO_BRANCH --build-arg M1M3_SUPPORT=$BRANCH "
            + "--build-arg TARGET=simulator "
            + (params.noCache ? " --no-cache " : " ") + "$WORKSPACE/ts_m1m3support"
        )
    }

    stage("Fetching current versions") {
        withEnv(["SALUSER_HOME=" + SALUSER_HOME]) {
            M1M3sim.inside("--entrypoint=''") {
                sh """
                    source $SALUSER_HOME/.crio_setup.sh

                    cd $WORKSPACE
                    [ -d ts_cRIOcpp ] || git clone --branch $CRIO_BRANCH https://github.com/lsst-ts/ts_cRIOcpp
                """
            }
        }
    }

    stage("Building binaries") {
        if (params.clean) {
            withEnv(["SALUSER_HOME=" + SALUSER_HOME]) {
                M1M3sim.inside("--entrypoint=''") {
                    sh """
                        source $SALUSER_HOME/.crio_setup.sh

                        cd $WORKSPACE/ts_cRIOcpp
                        git fetch
                        git checkout $CRIO_BRANCH
                        git pull
                        make clean

                        cd $WORKSPACE/ts_m1m3support
                        make clean
                    """
                }
            }
        }

        withEnv(["SALUSER_HOME=" + SALUSER_HOME]) {
            M1M3sim.inside("--entrypoint=''") {
                sh """
                    source $SALUSER_HOME/.crio_setup.sh

                    cd $WORKSPACE/ts_cRIOcpp
                    make -j\$(nproc)

                    cd $WORKSPACE/ts_m1m3support
                    make -j\$(nproc) SIMULATOR=1
                """
            }
        }
    }

    stage("Running tests")
    {
        withEnv(["SALUSER_HOME=" + SALUSER_HOME]) {
            M1M3sim.inside("--entrypoint=''") {
                sh """
                    source $SALUSER_HOME/.crio_setup.sh
                    cd $WORKSPACE/ts_m1m3support
                    make -j\$(nproc) SIMULATOR=1 junit
                """
            }
        }

        junit 'ts_m1m3support/tests/*.xml'
    }

    stage('Build documentation')
    {
         M1M3sim.inside("--entrypoint=''") {
             sh """
                source $SALUSER_HOME/.crio_setup.sh

                mamba install -y doxygen
                pip install ltd-conveyor
                cd $WORKSPACE/ts_m1m3support
                make doc
             """
         }
    }

    if (BRANCH == "main" || BRANCH == "develop")
    {
        stage('Publish documentation')
        {
            withCredentials([usernamePassword(credentialsId: 'lsst-io', usernameVariable: 'LTD_USERNAME', passwordVariable: 'LTD_PASSWORD')]) {
                M1M3sim.inside("--entrypoint=''") {
                    sh """
                        source $SALUSER_HOME/.crio_setup.sh
                        cd $WORKSPACE
                        ltd upload --product ts-m1m3support --git-ref """ + BRANCH + """ --dir $WORKSPACE/ts_m1m3support/doc/html
                    """
                }
            }
        }
    }
}
