pipeline {

    environment {
        KLOCWORK_SERVER = "http://klocwork.default.svc.cluster.local:8080"
        KLOCWORK_PROJECT = "cpp-project-template"
    }

    parameters {
        booleanParam(name: 'KLOCWORK_STATIC_ANALYSIS', defaultValue: true, description: 'run klocwork static analyzer')
    }

    agent {
        kubernetes {
          label 'buildagent-cpp-project-template'
          defaultContainer 'jnlp'
          yamlFile './klockworkKubernetes.yaml'
        }
    }

stages {
    stage('Prerequisites') {
        when {
            expression { return params.KLOCWORK_STATIC_ANALYSIS }
        }
        steps {
            container('buildagent') {
              sh """#!/usr/bin/env bash
                sudo hostname buildagent01
                sudo /bin/sh -c 'echo "127.0.0.1 buildagent01" >> /etc/hosts'
              """
            }
        }
    }

    stage('Compilation') {
        failFast false
        parallel {
            stage('Static') {
                stages {
                    stage('Makefile Generation') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu

                                    mkdir build-static-analysis
                                    cd build-static-analysis
                                    cmake -DCONAN_PROFILE=default            \
                                            -DUSE_CLANG_TIDY=ON ../
                                """
                            }
                        }
                    }

                    stage('Build with no Klocwork Analysis') {
                        when {
                            expression { return !params.KLOCWORK_STATIC_ANALYSIS }
                        }
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    cd build-static-analysis

                                    make -j8
                                """
                            }
                        }
                    }

                    stage('Build with Klocwork Analysis') {
                        when {
                            expression { return params.KLOCWORK_STATIC_ANALYSIS }
                        }
                        steps {
                            container('buildagent') {
                            sh """#!/usr/bin/env bash
                                set -exu
                                export PATH=/home/kw/dist181/bin:\$PATH
                                cd build-static-analysis

                                kwdeploy sync --url ${env.KLOCWORK_SERVER}
                                kwinject cmake --build . -- -j12
                                kwbuildproject -tables-directory kwtables --force \
                                    --url ${env.KLOCWORK_SERVER}/${env.KLOCWORK_PROJECT} kwinject.out -j8 \
                                    || echo "Exit code is not zero"
                                kwadmin --url ${env.KLOCWORK_SERVER} load ${env.KLOCWORK_PROJECT} kwtables \
                                    || echo "Exit code is not zero"
                            """
                            }
                        }
                    }
                    stage('Tests') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    cd build-static-analysis
                                    ctest -- -j12
                                    """
                            }
                        }
                    }
                }
            }
            stage('ASan') {
                stages {
                    stage('Makefile Generation') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    mkdir build-asan
                                    cd build-asan
                                    cmake -DCONAN_PROFILE=default \
                                            -DUSE_CLANG_TIDY=OFF    \
                                            -DUSE_CPPCHECK=OFF      \
                                            -DSANITIZE_ADDRESS=ON ../
                                """
                            }
                        }
                    }
                    stage('Compilation') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    cd build-asan
                                    cmake --build . -- -j12
                                    """
                            }
                        }
                    }
                    stage('Tests') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    cd build-asan
                                    ctest -j12
                                """
                            }
                        }
                    }
                }
            }
            stage('UBTSan') {
                stages {
                    stage('Makefile Generation') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    mkdir build-ubsan-tsan
                                    cd build-ubsan-tsan
                                    cmake -DCONAN_PROFILE=default  \
                                            -DUSE_CLANG_TIDY=OFF     \
                                            -DUSE_CPPCHECK=OFF       \
                                            -DSANITIZE_ADDRESS=OFF   \
                                            -DSANITIZE_UNDEFINED=ON  \
                                            -DSANITIZE_THREAD=ON ../
                                """
                            }
                        }
                    }
                    stage('Compilation') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    cd build-ubsan-tsan
                                    cmake --build . -- -j12
                                """
                            }
                        }
                    }
                    stage('Tests') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    cd build-ubsan-tsan
                                    ctest -j12
                                """
                            }
                        }
                    }
                }
            }

            stage('Security Checks') {
                stages {
                    stage('Makefile Generation') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    mkdir build-security
                                    cd build-security
                                    cmake -DCMAKE_BUILD_TYPE=Debug     \
                                            -DCONAN_PROFILE=default      \
                                            -DSANITIZE_ADDRESS=OFF       \
                                            -DCOMPILER_FORTIFY_SOURCE=ON ../
                                """
                            }
                        }
                    }
                    stage('Compilation') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    cd build-security
                                    cmake --build . -- -j12
                                """
                            }
                        }
                    }
                    stage('Tests') {
                        steps {
                            container('buildagent') {
                                sh """#!/usr/bin/env bash
                                    set -exu
                                    cd build-security
                                    ctest -j12
                                """
                            }
                        }
                    }
                }
            }
        }
    }   

    stage('Style Verification') {
        failFast false
        parallel {
            stage('clang-format') {
                steps {
                    container('buildagent') {
                        sh('./scripts/check-clang-format.sh')
                    }
                }
            }
            stage('cmake-format') {
                steps {
                    container('buildagent') {
                        sh('./scripts/check-cmake-format.sh')
                    }
                }
            }
        }
    }

} // stages

} // pipeline
