main(argc: int, argv: **byte, envp: **byte) {
	var i: int;
	i = 1;
	loop {
		if i >= argc {
			return;
		}

		if unlink(argv[i]) != 0 {
			die("rm failed");
		}

		i = i + 1;
	}
}
