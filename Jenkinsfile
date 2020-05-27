#!/usr/bin/env groovy

node {

    def M1M3sim
    def SAL_REPOS = "/home/saluser/repos"

    stage('Cloning Dockerfile')
    {
        sh "printenv"

        dir("ts_Dockerfiles") {
            git branch: "tickets/DM-25064", url: 'https://github.com/lsst-ts/ts_Dockerfiles'
        }
    }

    stage('Cloning source')
    {
        git branch: ((env.CHANGE_BRANCH != null) ? env.CHANGE_BRANCH : env.BRANCH_NAME), url: 'https://github.com/lsst-ts/ts_m1m3support'
    }

    stage('Building dev container')
    {
        M1M3sim = docker.build("lsstts/mtm1m3_sim:${env.BRANCH_NAME}", "--target lsstts-cpp-dev ts_Dockerfiles/mtm1m3_sim")
    }

    stage("Running tests")
    {
         M1M3sim.inside("--entrypoint=''") {
            sh.withEnv(["SAL_REPOS=" + SAL_REPOS]) '''
                cd $SAL_REPOS
                source ts_sal/setup.env

                export PATH=/opt/rh/devtoolset-8/root/usr/bin:$PATH

                cd $WORKSPACE
                make clean
                make SIMULATOR=1
                make run_tests
            '''
         }
    }

    stage('Running container')
    {
        M1M3sim.inside("--entrypoint=''") {
            sh.withEnv(["SAL_REPOS=" + SAL_REPOS]) """
                cd $SAL_REPOS
                source ts_sal/setup.env

                cd $WORKSPACE
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