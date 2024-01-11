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
    // branches requiring changes in XML from default develop branch
    def XML_BRANCH = BRANCH in ["main", "tickets/DM-39576"] ? BRANCH : "develop"
    def CRIO_BRANCH = BRANCH in ["main", "tickets/DM-42413"] ? BRANCH : "develop"
    stage('Cloning sources')
    {
        dir("ts_cRIOcpp") {
            git branch: CRIO_BRANCH, url: 'https://github.com/lsst-ts/ts_cRIOcpp'
        }
        dir("ts_m1m3support") {
            git branch: BRANCH, url: 'https://github.com/lsst-ts/ts_m1m3support'
        }
    }

    stage('Building dev container')
    {
        M1M3sim = docker.build("m1m3sim:" + env.BRANCH_NAME.replace("/", "_"), "--target crio-develop --build-arg XML_BRANCH=$XML_BRANCH " + (params.noCache ? "--no-cache " : " ") + "$WORKSPACE/ts_m1m3support")
    }

    stage("Running tests")
    {
        withEnv(["SALUSER_HOME=" + SALUSER_HOME]) {
             M1M3sim.inside("--entrypoint=''") {
                 if (params.clean) {
                 sh """
                    cd $WORKSPACE/ts_cRIOcpp
                    make clean
                    cd $WORKSPACE/ts_m1m3support
                    make clean
                 """
                 }
                 sh """
                    source $SALUSER_HOME/.crio_setup.sh
    
                    cd $WORKSPACE/ts_cRIOcpp
                    make
    
                    cd $WORKSPACE/ts_m1m3support

                    make SIMULATOR=1
                    LSST_DDS_PARTITION_PREFIX=test make SIMULATOR=1 junit || true
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
                cd $WORKSPACE/ts_m1m3support
                make doc
             """
         }
    }

    stage('Running container')
    {
        withEnv(["SALUSER_HOME=" + SALUSER_HOME]){
            M1M3sim.inside("--entrypoint=''") {
                sh """
                    source $SALUSER_HOME/.crio_setup.sh

                    export LSST_DDS_PARTITION_PREFIX=test
    
                    cd $WORKSPACE/ts_m1m3support
                    ./ts-M1M3supportd -c SettingFiles -f &
    
                    echo "Waiting for 15 seconds"
                    sleep 15
    
                    cd tests
                    ./runSimulator.py
                    killall -9 ts-M1M3supportd
                """
            }
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
                        ltd upload --product ts-m1m3support --git-ref """ + BRANCH + """ --dir $WORKSPACE/ts_m1m3support/doc/html
                    """
                }
            }
        }
    }
}
