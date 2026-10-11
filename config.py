def can_build(env, platform):
    return True


def configure(env):
    pass


def get_opts(platform):
    from SCons.Variables import EnumVariable

    return [
        EnumVariable(
            "bigfloat_backend",
            "BigFloat backend",
            "approx",
            ["approx", "precise"],
        )
    ]
