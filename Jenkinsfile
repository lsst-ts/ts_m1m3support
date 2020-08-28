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
    disableConcurrentBuilds()
    ]
)

node {

    def M1M3sim
    def SAL_REPOS = "/home/saluser/repos"
    def BRANCH = (env.CHANGE_BRANCH != null) ? env.CHANGE_BRANCH : env.BRANCH_NAME

    stage('Cloning Dockerfile')
    {
        sh "printenv"

        dir("ts_Dockerfiles") {
            git branch: (BRANCH == "master" ? "master" : "develop"), url: 'https://github.com/lsst-ts/ts_Dockerfiles'
        }
    }

    stage('Building dev container')
    {
        M1M3sim = docker.build("lsstts/mtm1m3_sim:" + env.BRANCH_NAME.replace("/", "_"), "--target lsstts-cpp-dev ts_Dockerfiles/mtm1m3_sim")
    }

    stage('Cloning source')
    {
        dir("ts_m1m3support") {
            git branch: BRANCH, url: 'https://github.com/lsst-ts/ts_m1m3support'
        }
    }

    stage("Running tests")
    {
        withEnv(["SAL_REPOS=" + SAL_REPOS]) {
             M1M3sim.inside("--entrypoint=''") {
                 sh '''
                    cd $SAL_REPOS
                    source ts_sal/setup.env
    
                    export PATH=/opt/rh/devtoolset-8/root/usr/bin:$PATH
    
                    cd $WORKSPACE/ts_m1m3support
                    make SIMULATOR=1
                    make junit
                '''
             }
        }

        junit 'ts_m1m3support/tests/*.xml'
    }

    stage('Build documentation')
    {
         M1M3sim.inside("--entrypoint=''") {
             sh '''
                source /home/saluser/.setup.sh
                cd $WORKSPACE/ts_m1m3support
                make doc
            '''
         }
    }

    stage('Running container')
    {
        withEnv(["SAL_REPOS=" + SAL_REPOS]){
            M1M3sim.inside("--entrypoint=''") {
                sh """
                    cd $SAL_REPOS
                    source ts_sal/setup.env
    
                    cd $WORKSPACE/ts_m1m3support
                    ./ts_M1M3Support -c SettingFiles &
    
                    echo "Waiting for 30 seconds"
                    sleep 30
    
                    cd /home/saluser/repos
                    ./ts_sal/test/MTM1M3/cpp/src/sacpp_MTM1M3_start_commander Default
                    sleep 30
                    killall ts_M1M3Support
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
                        source /home/saluser/.setup.sh
                        ltd upload --product ts-m1m3support --git-ref """ + BRANCH + """ --dir $WORKSPACE/ts_m1m3support/doc/html
                    """
                }
            }
        }
    }
}
