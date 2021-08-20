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
    def XML_BRANCH = (BRANCH == "master") ? BRANCH : "develop"
    stage('Cloning sources')
    {
        dir("ts_cRIOcpp") {
            git branch: "master", url: 'https://github.com/lsst-ts/ts_cRIOcpp'
        }
        dir("ts_m1m3support") {
            git branch: BRANCH, url: 'https://github.com/lsst-ts/ts_m1m3support'
        }
    }

    stage('Building dev container')
    {
        M1M3sim = docker.build("m1m3sim:" + env.BRANCH_NAME.replace("/", "_"), "--build-arg XML_BRANCH=$XML_BRANCH " + (params.noCache ? "--no-cache " : " ") + "$WORKSPACE/ts_m1m3support")
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
                    source $SALUSER_HOME/.setup_salobj.sh
    
                    export PATH=\$CONDA_PREFIX/bin:$PATH
                    cd $WORKSPACE/ts_cRIOcpp
                    make
    
                    cd $WORKSPACE/ts_m1m3support
                    ls -l \$CONDA_PREFIX/include
                    ls -l \$CONDA_PREFIX/include/yaml-cpp
                    LIBS_FLAGS="-L\$CONDA_PREFIX/lib" SAL_CPPFLAGS="-I\$CONDA_PREFIX/include" PKG_CONFIG_PATH=\$CONDA_PREFIX/lib/pkgconfig make VERBOSE=1 simulator

                    LSST_DDS_PARTITION_PREFIX=test LIBS_FLAGS=-L\$CONDA_PREFIX/lib SAL_CPPFLAGS=-I\$CONDA_PREFIXinclude make junit || true
                 """
             }
        }

        junit 'ts_m1m3support/tests/*.xml'
    }

    stage('Build documentation')
    {
         M1M3sim.inside("--entrypoint=''") {
             sh """
                source $SALUSER_HOME/.setup_salobj.sh
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
                    source $SALUSER_HOME/.setup_salobj.sh

                    export LSST_DDS_PARTITION_PREFIX=test
    
                    cd $WORKSPACE/ts_m1m3support
                    ./ts-M1M3supportd -c SettingFiles &
    
                    echo "Waiting for 30 seconds"
                    sleep 30
    
                    cd $SALUSER_HOME/repos
                    ./ts_sal/test/MTM1M3/cpp/src/sacpp_MTM1M3_start_commander Default
                    sleep 30
                    killall ts-M1M3supportd
                """
            }
        }
    }

    if (BRANCH == "master" || BRANCH == "develop")
    {
        stage('Publish documentation')
        {
            withCredentials([usernamePassword(credentialsId: 'lsst-io', usernameVariable: 'LTD_USERNAME', passwordVariable: 'LTD_PASSWORD')]) {
                M1M3sim.inside("--entrypoint=''") {
                    sh """
                        source $SALUSER_HOME/.setup_salobj.sh
                        ltd upload --product ts-m1m3support --git-ref """ + BRANCH + """ --dir $WORKSPACE/ts_m1m3support/doc/html
                    """
                }
            }
        }
    }
}
